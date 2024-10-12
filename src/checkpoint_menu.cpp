#include "checkpoint_menu.h"
#include <glm/ext/matrix_transform.hpp>
#include <scripts_system.h>
#include "graphics_menu.h"
#include "pause_menu.h"
#include <gameplay_manager.h>
#include <time_system.h>
#include <engine.h>

game::checkpoint_menu* game::checkpoint_menu::instance = nullptr;

game::checkpoint_menu::checkpoint_menu() :
	title("CHECKPOINT", "../assets/fonts/bitmap/handwiriting-readable.png",
		glm::vec3(0.5f, 0.95f, 0.02f), glm::scale(glm::mat4(1.0f), glm::vec3(0.03f, 0.05f, 1.0f))),
	back(glm::vec3(0.5f, 0.1f, 0.02f), glm::vec2(0.07f, 0.03f), "../assets/textures/White_Square.png", "BACK", "../assets/fonts/bitmap/handwiriting-readable.png")
{
	// singleton stuff
	if (game::checkpoint_menu::instance) {
		printf("soft error: attempt to create second checkpoint menu\n");
		delete this;
		return;
	}
	game::checkpoint_menu::instance = this;

	// STYLE
	// back
	back.text.color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

	// FUNCTION
	this->back.on_click.subscribe([]() { scripts_system::safe_destroy(game::checkpoint_menu::instance); });
}

game::checkpoint_menu::~checkpoint_menu()
{
	time_system::time_scale = 1.0;
	glfwSetInputMode(engine::window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	if (game::checkpoint_menu::instance == this) game::checkpoint_menu::instance = nullptr;
}
