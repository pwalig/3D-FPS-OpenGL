#include "gameplay_manager.h"
#include <time_system.h>
#include <input_system.h>
#include <scene_loader.h>
#include <pbr_model.h>
#include <engine.h>
#include <enemy_generator.h>
#include <debug_defines.h>

#ifdef DEBUG
#include <stdexcept>
#endif

game::gameplay_manager* game::gameplay_manager::instance = nullptr;
glm::vec3* game::gameplay_manager::player_position = nullptr;
bool game::gameplay_manager::game_paused = false;
double game::gameplay_manager::_time_scale_buffor = 1.0f;
float game::gameplay_manager::difficulty_float = 1.0f;

game::gameplay_manager::gameplay_manager()
{
	if (instance) {
		delete this;
		return;
	}
	instance = this;
	game::gameplay_manager::_time_scale_buffor = time_system::time_scale;
	scene_loader::generator::init();
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
		glfwSwapInterval(game::graphics_menu::vsynch_enabled ? 1 : 0);
	}
	else {
		// Prze³¹cz na tryb okienkowy
		glfwSetWindowMonitor(engine::window, NULL, engine::windowed_xpos, engine::windowed_ypos, engine::window_width, engine::window_height, 0);
		glViewport(0, 0, windowed_width, windowed_height);
		renderer::active_camera.set_aspect_ratio(engine::window_width / engine::window_height);
	}
}

game::gameplay_manager::~gameplay_manager()
{
	if (instance == this) instance = nullptr;
}

float game::gameplay_manager::multiply_by_difficulty(const float& value, const float& influence, const bool& inverse)
{
	if (difficulty_float == 0.0f) {
		if (inverse) return std::numeric_limits<float>::max();
		else return 0.0f;
	}
#ifdef DEBUG
	if (influence > 1.0f || influence < 0.0f) throw std::runtime_error("influence was not in range (0.0f ; 1.0f)");
#endif
	float multiplier = difficulty_float;
	if (inverse) multiplier = 1.0f / multiplier;
	float new_value = value * multiplier;
	return (new_value * influence) + (value * (1.0f - influence));
}

int game::gameplay_manager::multiply_by_difficulty(const int& value, float influence, const bool& inverse)
{
	return (int)multiply_by_difficulty((float)value, influence, inverse);
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
