#include "title_screen.h"
#include <glm/ext/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include <scripts_system.h>
#include "main_menu.h"
#include <scene_loader.h>

game::title_screen* game::title_screen::instance = nullptr;

game::title_screen::title_screen() :
	text("PRESS ANY KEY TO START", "../assets/fonts/bitmap/handwiriting-readable.png",
		glm::vec3(0.5f, 0.2f, 0.1f), glm::scale(glm::mat4(1.0f), glm::vec3(0.03f, 0.05f, 0.01f))),
	proceed_key_bind([this]() {
		scene_loader::move_same_scene(new game::main_menu(), this);
		scripts_system::safe_destroy(this);
	}, 31, GLFW_PRESS)
{
	// singleton stuff
	if (instance) {
		printf("soft error: attempt to create second main menu\n");
		delete this;
		return;
	}
	instance = this;
}

game::title_screen::~title_screen()
{
	if (instance == this) instance = nullptr;
}
