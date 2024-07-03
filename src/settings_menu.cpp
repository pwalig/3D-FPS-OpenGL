#include "settings_menu.h"
#include <glm/ext/matrix_transform.hpp>
#include <scripts_system.h>
#include "graphics_menu.h"
#include "pause_menu.h"
#include <gameplay_manager.h>

game::settings_menu* game::settings_menu::instance = nullptr;

game::settings_menu::settings_menu(const std::function<void()>& on_close) :
	title("SETTINGS", "../assets/fonts/bitmap/handwiriting-readable.png",
	glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(800.0f, 1080.0f, -1.0f)), glm::vec3(30.0f, 50.0f, 1.0f))),
	volume_text("VOLUME", "../assets/fonts/bitmap/handwiriting-readable.png",
	glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(800.0f, 1000.0f, -1.0f)), glm::vec3(17.0f, 30.0f, 1.0f))),
	difficulty_text("DIFFICULTY : x" + std::to_string(game::gameplay_manager::difficulty_float), "../assets/fonts/bitmap/handwiriting-readable.png",
	glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(800.0f, 850.0f, -1.0f)), glm::vec3(17.0f, 30.0f, 1.0f))),
	mouse_sensitivity_text("MOUSE SENSITIVITY : " + std::to_string(input_system::global_mouse_sensitivity * 300.0f), "../assets/fonts/bitmap/handwiriting-readable.png",
	glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(800.0f, 700.0f, -1.0f)), glm::vec3(17.0f, 30.0f, 1.0f))),
	volume(glm::vec3(960.0f, 925.0f, -10.0f), glm::vec2(120.0f, 20.0f)),
	difficulty(glm::vec3(960.0f, 775.0f, -10.0f), glm::vec2(120.0f, 20.0f)),
	mouse_sensitivity(glm::vec3(960.0f, 625.0f, -10.0f), glm::vec2(120.0f, 20.0f)),
	back(glm::vec3(960.0f, 100.0f, -10.0f), glm::vec2(75.0f, 32.0f), "../assets/textures/White_Square.png", "BACK", "../assets/fonts/bitmap/handwiriting-readable.png"),
	graphics(glm::vec3(960.0f, 500.0f, -10.0f), glm::vec2(75.0f, 32.0f), "../assets/textures/White_Square.png", "GRAPHICS", "../assets/fonts/bitmap/handwiriting-readable.png")
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
