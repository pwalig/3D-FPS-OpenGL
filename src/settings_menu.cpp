#include "settings_menu.h"
#include <glm/ext/matrix_transform.hpp>
#include <scripts_system.h>

game::settings_menu* game::settings_menu::instance = nullptr;

game::settings_menu::settings_menu(const std::function<void()>& on_close) : title("SETTINGS", "../assets/fonts/bitmap/handwiriting-readable.png",
	glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(800.0f, 1000.0f, -1.0f)), glm::vec3(30.0f, 50.0f, 1.0f))),
	volume_text("VOLUME", "../assets/fonts/bitmap/handwiriting-readable.png",
	glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(800.0f, 900.0f, -1.0f)), glm::vec3(30.0f, 50.0f, 1.0f))),
	difficulty_text("DIFFICULTY", "../assets/fonts/bitmap/handwiriting-readable.png",
	glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(800.0f, 700.0f, -1.0f)), glm::vec3(30.0f, 50.0f, 1.0f))),
	volume(glm::vec3(960.0f, 800.0f, -10.0f), glm::vec2(120.0f, 20.0f)),
	difficulty(glm::vec3(960.0f, 600.0f, -10.0f), glm::vec2(120.0f, 20.0f)),
	back(glm::vec3(960.0f, 100.0f, -10.0f), glm::vec2(75.0f, 32.0f), "../assets/textures/White_Square.png", "BACK", "../assets/fonts/bitmap/handwiriting-readable.png"),
	uid(glm::vec3(960.0f, 500.0f, -10.0f), glm::vec2(75.0f, 32.0f), "../assets/textures/White_Square.png", "DROPDOWN", "../assets/fonts/bitmap/handwiriting-readable.png"),
	uit(glm::vec3(960.0f, 400.0f, -10.0f), glm::vec2(75.0f, 32.0f), "../assets/textures/White_Square.png", "input text", "../assets/fonts/bitmap/handwiriting-readable.png"),
	uic(glm::vec3(960.0f, 200.0f, -10.0f), glm::vec2(30.0f, 30.0f))
{
	// singleton stuff
	if (game::settings_menu::instance) {
		printf("soft error: attempt to create second settings menu\n");
		delete this;
		return;
	}
	game::settings_menu::instance = this;


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
	this->uid.values.push_back("DROPDOWN");
	this->uid.values.push_back("YEET");
	this->uid.values.push_back("NUH UH");
	this->uid.values.push_back("YUSSSSS");
	this->back.on_click.subscribe(on_close);
	this->back.on_click.subscribe([](){ scripts_system::safe_destroy(game::settings_menu::instance); });
}

game::settings_menu::~settings_menu()
{
	if (game::settings_menu::instance == this) game::settings_menu::instance = nullptr;
}