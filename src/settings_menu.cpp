#include "settings_menu.h"
#include <glm/ext/matrix_transform.hpp>
#include <scripts_system.h>
#include "graphics_menu.h"
#include "pause_menu.h"
#include <gameplay_manager.h>

game::settings_menu* game::settings_menu::instance = nullptr;

game::settings_menu::settings_menu(const std::function<void()>& on_close) :
	title("SETTINGS", "../assets/fonts/bitmap/handwiriting-readable.png",
		glm::vec3(0.5f, 0.95f, 0.02f), glm::scale(glm::mat4(1.0f), glm::vec3(0.03f, 0.05f, 1.0f))),
	volume_text("VOLUME", "../assets/fonts/bitmap/handwiriting-readable.png",
		glm::vec3(0.5f, 0.9f, 0.02f), glm::scale(glm::mat4(1.0f), glm::vec3(0.017f, 0.03f, 1.0f))),
	difficulty_text("DIFFICULTY : x" + std::to_string(game::gameplay_manager::difficulty_float), "../assets/fonts/bitmap/handwiriting-readable.png",
		glm::vec3(0.5f, 0.8f, 0.02f), glm::scale(glm::mat4(1.0f), glm::vec3(0.017f, 0.03f, 1.0f))),
	mouse_sensitivity_text("MOUSE SENSITIVITY : " + std::to_string(input_system::global_mouse_sensitivity * 300.0f), "../assets/fonts/bitmap/handwiriting-readable.png",
		glm::vec3(0.5f, 0.7f, 0.02f), glm::scale(glm::mat4(1.0f), glm::vec3(0.017f, 0.03f, 1.0f))),
	volume(glm::vec3(0.5f, 0.85f, 0.02f), glm::vec2(0.12f, 0.02f)),
	difficulty(glm::vec3(0.5f, 0.75f, 0.02f), glm::vec2(0.12f, 0.02f)),
	mouse_sensitivity(glm::vec3(0.5f, 0.65f, 0.02f), glm::vec2(0.12f, 0.02f)),
	back(glm::vec3(0.5f, 0.1f, 0.02f), glm::vec2(0.07f, 0.03f), "../assets/textures/White_Square.png", "BACK", "../assets/fonts/bitmap/handwiriting-readable.png"),
	graphics(glm::vec3(0.5f, 0.5f, 0.02f), glm::vec2(0.07f, 0.03f), "../assets/textures/White_Square.png", "GRAPHICS", "../assets/fonts/bitmap/handwiriting-readable.png")
{
	// singleton stuff
	if (game::settings_menu::instance) {
		printf("soft error: attempt to create second settings menu\n");
		delete this;
		return;
	}
	game::settings_menu::instance = this;

	// DEFAULTS
	this->mouse_sensitivity.value = input_system::global_mouse_sensitivity * 300.0f;
	this->mouse_sensitivity.update_visual();
	this->difficulty.value = std::sqrt(game::gameplay_manager::difficulty_float / 10.0f);
	this->difficulty.update_visual();

	// STYLE
	// volume
	volume.fill_hover_color = glm::vec4(0.65f, 0.65f, 0.65f, 1.0f);
	volume.fill_hold_color = glm::vec4(0.6f, 0.6f, 0.6f, 1.0f);
	volume.handle_hover_color = glm::vec4(0.9f, 0.9f, 0.9f, 1.0f);
	volume.handle_hold_color = glm::vec4(0.8f, 0.8f, 0.8f, 1.0f);
	volume.background_hover_color = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);
	volume.background_hold_color = glm::vec4(0.1f, 0.1f, 0.1f, 1.0f);
	// difficulty
	difficulty.fill_hover_color = glm::vec4(0.65f, 0.65f, 0.65f, 1.0f);
	difficulty.fill_hold_color = glm::vec4(0.6f, 0.6f, 0.6f, 1.0f);
	difficulty.handle_hover_color = glm::vec4(0.9f, 0.9f, 0.9f, 1.0f);
	difficulty.handle_hold_color = glm::vec4(0.8f, 0.8f, 0.8f, 1.0f);
	difficulty.background_hover_color = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);
	difficulty.background_hold_color = glm::vec4(0.1f, 0.1f, 0.1f, 1.0f);
	// back
	back.text.color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

	// FUNCTION
	this->mouse_sensitivity.on_value_changed = [this](float new_sensitivity) {
		input_system::global_mouse_sensitivity = new_sensitivity / 300.0f;
		this->mouse_sensitivity_text.text = "MOUSE SENSITIVITY : " + std::to_string(new_sensitivity);
		};
	this->difficulty.on_value_changed = [this](float new_difficulty) {
		game::gameplay_manager::difficulty_float = new_difficulty * new_difficulty * 10.0f;
		this->difficulty_text.text = "DIFFICULTY : x" + std::to_string(game::gameplay_manager::difficulty_float);
		};
	this->graphics.on_click.subscribe([on_close]() {
		new graphics_menu([on_close]() { new game::settings_menu([on_close]() { on_close(); }); });
		scripts_system::safe_destroy(game::settings_menu::instance);
		});
	this->back.on_click.subscribe([](){ scripts_system::safe_destroy(game::settings_menu::instance); });
	this->back.on_click.subscribe(on_close);
}

game::settings_menu::~settings_menu()
{
	if (game::settings_menu::instance == this) game::settings_menu::instance = nullptr;
}
