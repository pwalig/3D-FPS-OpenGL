#include "gameplay_manager.h"
#include <time_system.h>
#include <input_system.h>
#include <scene_loader.h>
#include <pbr_model.h>
#include <engine.h>

glm::vec3* game::gameplay_manager::player_position = nullptr;
bool game::gameplay_manager::game_paused = false;

game::gameplay_manager::gameplay_manager()
	: ms(new renderer::pbr_model(
		"../assets/models/jet_axe.obj",
		"../assets/textures/jet_axe/Axe_Normal.png",
		"../assets/textures/jet_axe/Axe_Albedo.png",
		"../assets/textures/White_Square.png",
		glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 3.0f, 0.0f)), glm::vec3(10.0f))
	)),
	_time_scale_buffor(1.0f)
{
	this->_time_scale_buffor = time_system::time_scale;
}

void game::gameplay_manager::start()
{
}

void game::gameplay_manager::pause_un_pause()
{
	if (game::gameplay_manager::game_paused) this->un_pause();
	else this->pause();
}

void game::gameplay_manager::pause()
{
	this->_time_scale_buffor = time_system::time_scale;
	time_system::time_scale = 0.0f;
	game::gameplay_manager::game_paused = true;
	glfwSetInputMode(engine::window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void game::gameplay_manager::un_pause()
{
	time_system::time_scale = this->_time_scale_buffor;
	game::gameplay_manager::game_paused = false;
	glfwSetInputMode(engine::window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void game::gameplay_manager::full_screen()
{
	engine::is_fullscreen = !engine::is_fullscreen;
	int windowed_width = static_cast<int>(engine::window_width);
	int windowed_height = static_cast<int>(engine::window_height);

	if (engine::is_fullscreen) {
		glfwGetWindowPos(engine::window, &engine::windowed_xpos, &engine::windowed_ypos);
		glfwGetWindowSize(engine::window, &windowed_width, &windowed_height);

		// Uzyskaj tryb wideo g³ównego monitora
		GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* mode = glfwGetVideoMode(primaryMonitor);

		// Prze³¹cz na pe³ny ekran
		glfwSetWindowMonitor(engine::window, primaryMonitor, 0, 0, mode->width, mode->height, mode->refreshRate);
		glViewport(0, 0, mode->width, mode->height);
	}
	else {
		// Prze³¹cz na tryb okienkowy
		glfwSetWindowMonitor(engine::window, NULL, engine::windowed_xpos, engine::windowed_ypos, engine::window_width, engine::window_height, 0);
		glViewport(0, 0, windowed_width, windowed_height);
	}
}
