#include "main_menu.h"
#include <engine.h>
#include <scene_loader.h>
#include "settings_menu.h"

game::main_menu* game::main_menu::instance = nullptr;

game::main_menu::main_menu() :
	new_game(glm::vec3(0.5f, 0.7f, 0.02f), glm::vec2(0.12f, 0.03f), "../assets/textures/White_Square.png", "NEW GAME", "../assets/fonts/bitmap/handwiriting-readable.png"),
	continue_(glm::vec3(0.5f, 0.6f, 0.02f), glm::vec2(0.12f, 0.03f), "../assets/textures/White_Square.png", "CONTINUE", "../assets/fonts/bitmap/handwiriting-readable.png"),
	load_game(glm::vec3(0.5f, 0.5f, 0.02f), glm::vec2(0.12f, 0.03f), "../assets/textures/White_Square.png", "LOAD GAME", "../assets/fonts/bitmap/handwiriting-readable.png"),
	settings(glm::vec3(0.5f, 0.4f, 0.02f), glm::vec2(0.12f, 0.03f), "../assets/textures/White_Square.png", "SETTINGS", "../assets/fonts/bitmap/handwiriting-readable.png"),
	quit(glm::vec3(0.5f, 0.3f, 0.02f), glm::vec2(0.12f, 0.03f), "../assets/textures/White_Square.png", "QUIT GAME", "../assets/fonts/bitmap/handwiriting-readable.png")
{
	// singleton stuff
	if (instance) {
		printf("soft error: attempt to create second main menu\n");
		delete this;
		return;
	}
	instance = this;

	// FUNCTION
	std::function<void()> load_backrooms = [this]() {
		scene_loader::load_scene("../assets/scenes/backrooms/backrooms.json");
		glfwSetInputMode(engine::window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		scripts_system::events[SCRIPTS_START].subscribe([this]() {
			scene_loader::un_load_scene(scene_loader::get_scene_name(this));
			});
		};
	new_game.on_click.subscribe(load_backrooms);
	continue_.on_click.subscribe(load_backrooms);
	settings.on_click.subscribe([this]() {
		std::string scene_name = scene_loader::get_scene_name(this);
		scene_loader::move_to_scene(new game::settings_menu([scene_name]() {
			scene_loader::move_to_scene(new game::main_menu(), scene_name);
			}), scene_name);
		scripts_system::safe_destroy(this);
		});
	quit.on_click.subscribe([]() { glfwSetWindowShouldClose(engine::window, GL_TRUE); });
}

game::main_menu::~main_menu()
{
	if (instance == this) instance = nullptr;
}