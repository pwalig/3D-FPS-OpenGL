#include "game_over_menu.h"
#include <engine.h>
#include <scene_loader.h>
#include "settings_menu.h"
#include "time_system.h"

game::game_over_menu* game::game_over_menu::instance = nullptr;

game::game_over_menu::game_over_menu() :
	title("GAME OVER", "../assets/fonts/bitmap/handwiriting-readable.png",
		glm::vec3(0.5f, 0.9, 0.02f), glm::scale(glm::mat4(1.0f), glm::vec3(0.03f, 0.05f, 1.0f))),
	retry(glm::vec3(0.5f, 0.7f, 0.02f), glm::vec2(0.12f, 0.03f), "../assets/textures/White_Square.png", " RETRY ", "../assets/fonts/bitmap/handwiriting-readable.png"),
	settings(glm::vec3(0.5f, 0.6f, 0.02f), glm::vec2(0.12f, 0.03f), "../assets/textures/White_Square.png", "SETTINGS", "../assets/fonts/bitmap/handwiriting-readable.png"),
	main_menu(glm::vec3(0.5f, 0.5f, 0.02f), glm::vec2(0.12f, 0.03f), "../assets/textures/White_Square.png", "MAIN MENU", "../assets/fonts/bitmap/handwiriting-readable.png"),
	quit(glm::vec3(0.5f, 0.4f, 0.02f), glm::vec2(0.12f, 0.03f), "../assets/textures/White_Square.png", " QUIT ", "../assets/fonts/bitmap/handwiriting-readable.png")
{
	if (instance) {
		delete this;
		return;
	}
	instance = this;
	time_system::time_scale = 0.0f;
	glfwSetInputMode(engine::window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	retry.on_click.subscribe([this]() {
		scene_loader::free();
		scene_loader::load_scene("../assets/scenes/backrooms/backrooms.json");
		glfwSetInputMode(engine::window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		time_system::time_scale = 1.0f;
		scripts_system::safe_destroy(this);
		});

	settings.on_click.subscribe([this]() {
		scripts_system::safe_destroy(this);
		new settings_menu([]() {new game_over_menu(); });
		});

	main_menu.on_click.subscribe([this]() {
		scene_loader::free();
		scene_loader::load_scene("initial_scene.json");
		time_system::time_scale = 1.0f;
		scripts_system::safe_destroy(this);
		});

	quit.on_click.subscribe([]() { glfwSetWindowShouldClose(engine::window, GL_TRUE); });
}

game::game_over_menu::~game_over_menu()
{
	if (instance == this) instance = nullptr;
}
