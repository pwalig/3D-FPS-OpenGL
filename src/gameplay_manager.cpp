#include "gameplay_manager.h"
#include <time_system.h>
#include <input_system.h>
#include <scene_loader.h>
#include <pbr_model.h>
#include <engine.h>

glm::vec3* game::gameplay_manager::player_position = nullptr;
bool game::gameplay_manager::game_paused = false;
double game::gameplay_manager::_time_scale_buffor = 1.0f;

game::gameplay_manager::gameplay_manager()
{
	game::gameplay_manager::_time_scale_buffor = time_system::time_scale;
}

void game::gameplay_manager::start()
{
}

void game::gameplay_manager::pause_un_pause()
{
	if (game::gameplay_manager::game_paused) game::gameplay_manager::un_pause();
	else game::gameplay_manager::pause();
}

void game::gameplay_manager::pause()
{
	game::gameplay_manager::_time_scale_buffor = time_system::time_scale;
	time_system::time_scale = 0.0f;
	game::gameplay_manager::game_paused = true;
	glfwSetInputMode(engine::window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	new game::pause_menu();
}

void game::gameplay_manager::un_pause()
{
	time_system::time_scale = game::gameplay_manager::_time_scale_buffor;
	game::gameplay_manager::game_paused = false;
	glfwSetInputMode(engine::window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	scripts_system::safe_destroy(game::pause_menu::instance);
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
		renderer::active_camera.set_aspect_ratio((float)mode->width / (float)mode->height);
	}
	else {
		// Prze³¹cz na tryb okienkowy
		glfwSetWindowMonitor(engine::window, NULL, engine::windowed_xpos, engine::windowed_ypos, engine::window_width, engine::window_height, 0);
		glViewport(0, 0, windowed_width, windowed_height);
		renderer::active_camera.set_aspect_ratio(engine::window_width / engine::window_height);
	}
}

void game::gameplay_manager::framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	renderer::active_camera.set_aspect_ratio((float)width / (float)height);
	glViewport(0, 0, width, height);
	if (!engine::is_fullscreen) {
		engine::window_width = width;
		engine::window_height = height;
	}
	engine::width = width;
	engine::height = height;
}
