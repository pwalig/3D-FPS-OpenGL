#include "pause_menu.h"
#include <gameplay_manager.h>
#include <engine.h>
#include "settings_menu.h"

game::pause_menu* game::pause_menu::instance = nullptr;

game::pause_menu::pause_menu() :
	paused("GAME PAUSED", "../assets/fonts/bitmap/handwiriting-readable.png",
		glm::vec3(0.5f, 0.7f, 0.02f), glm::scale(glm::mat4(1.0f), glm::vec3(0.03f, 0.05f, 1.0f))),
	un_pause(glm::vec3(0.5f, 0.6f, 0.02f), glm::vec2(0.12f, 0.03f), "../assets/textures/White_Square.png", "UN PAUSE", "../assets/fonts/bitmap/handwiriting-readable.png"),
	settings(glm::vec3(0.5f, 0.5f, 0.02f), glm::vec2(0.12f, 0.03f), "../assets/textures/White_Square.png", "SETTINGS", "../assets/fonts/bitmap/handwiriting-readable.png"),
	main_menu(glm::vec3(0.5f, 0.4f, 0.02f), glm::vec2(0.12f, 0.03f), "../assets/textures/White_Square.png", "MAIN MENU", "../assets/fonts/bitmap/handwiriting-readable.png"),
	quit(glm::vec3(0.5f, 0.3f, 0.02f), glm::vec2(0.12f, 0.03f), "../assets/textures/White_Square.png", "QUIT GAME", "../assets/fonts/bitmap/handwiriting-readable.png")
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
	un_pause.text.color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	un_pause.hover_color = glm::vec4(0.85f, 0.85f, 0.85f, 1.0f);
	un_pause.hold_color = glm::vec4(0.7f, 0.7f, 0.7f, 1.0f);
	// settings
	settings.text.color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	settings.hover_color = glm::vec4(0.85f, 0.85f, 0.85f, 1.0f);
	settings.hold_color = glm::vec4(0.7f, 0.7f, 0.7f, 1.0f);
	// main_menu
	main_menu.text.color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	main_menu.hover_color = glm::vec4(0.85f, 0.85f, 0.85f, 1.0f);
	main_menu.hold_color = glm::vec4(0.7f, 0.7f, 0.7f, 1.0f);
	// quit
	quit.text.color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	quit.hover_color = glm::vec4(0.85f, 0.85f, 0.85f, 1.0f);
	quit.hold_color = glm::vec4(0.7f, 0.7f, 0.7f, 1.0f);

	// FUNCTION
	// un_pause
	un_pause.on_click.subscribe([]() {game::gameplay_manager::un_pause(); });
	quit.on_click.subscribe([]() {glfwSetWindowShouldClose(engine::window, GL_TRUE); });
	settings.on_click.subscribe([]() {
		new game::settings_menu([]() { new game::pause_menu(); });
		scripts_system::safe_destroy(game::pause_menu::instance);
		});
	main_menu.on_click.subscribe([]() {
		gameplay_manager::un_pause();
		glfwSetInputMode(engine::window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		scripts_system::events[SCRIPTS_START].subscribe([]() {
			scene_loader::free();
			scene_loader::load_scene("initial_scene.json");
			});
		});
}

game::pause_menu::~pause_menu()
{
	if (game::pause_menu::instance == this) game::pause_menu::instance = nullptr;
}
