#include "pause_menu.h"
#include <gameplay_manager.h>
#include <engine.h>
#include "settings_menu.h"

game::pause_menu* game::pause_menu::instance = nullptr;

game::pause_menu::pause_menu() : paused("GAME PAUSED", "../assets/fonts/bitmap/handwiriting-readable.png",
	glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(800.0f, 700.0f, -1.0f)), glm::vec3(30.0f, 50.0f, 1.0f)))
{
	// singleton stuff
	if (game::pause_menu::instance) {
		printf("soft error: attempt to create second pause menu\n");
		delete this;
		return;
	}
	game::pause_menu::instance = this;

	// STYLE
	// un_pause
	un_pause.text.text = "UN PAUSE";
	un_pause.text.color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	un_pause.hover_color = glm::vec4(0.85f, 0.85f, 0.85f, 1.0f);
	un_pause.hold_color = glm::vec4(0.7f, 0.7f, 0.7f, 1.0f);
	un_pause.reposition(glm::vec3(960.0f, 610.0f, -1.0f), glm::vec2(120.0f, 32.0f));
	// settings
	settings.text.text = "SETTINGS";
	settings.text.color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	settings.hover_color = glm::vec4(0.85f, 0.85f, 0.85f, 1.0f);
	settings.hold_color = glm::vec4(0.7f, 0.7f, 0.7f, 1.0f);
	settings.reposition(glm::vec3(960.0f, 540.0f, -1.0f), glm::vec2(120.0f, 32.0f));
	// main_menu
	main_menu.text.text = "MAIN MENU";
	main_menu.text.color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	main_menu.hover_color = glm::vec4(0.85f, 0.85f, 0.85f, 1.0f);
	main_menu.hold_color = glm::vec4(0.7f, 0.7f, 0.7f, 1.0f);
	main_menu.reposition(glm::vec3(960.0f, 470.0f, -1.0f), glm::vec2(120.0f, 32.0f));
	// quit
	quit.text.text = "QUIT";
	quit.text.color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	quit.hover_color = glm::vec4(0.85f, 0.85f, 0.85f, 1.0f);
	quit.hold_color = glm::vec4(0.7f, 0.7f, 0.7f, 1.0f);
	quit.reposition(glm::vec3(960.0f, 400.0f, -1.0f), glm::vec2(120.0f, 32.0f));

	// FUNCTION
	// un_pause
	un_pause.on_click.subscribe([]() {game::gameplay_manager::un_pause(); });
	quit.on_click.subscribe([]() {glfwSetWindowShouldClose(engine::window, GL_TRUE); });
	settings.on_click.subscribe([]() {
		new game::settings_menu([]() { new game::pause_menu(); });
		scripts_system::safe_destroy(game::pause_menu::instance);
		});
}

game::pause_menu::~pause_menu()
{
	if (game::pause_menu::instance == this) game::pause_menu::instance = nullptr;
}
