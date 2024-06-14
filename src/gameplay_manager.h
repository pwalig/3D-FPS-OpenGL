#pragma once
#include "scripts_system.h"
#include "input_system.h"
#include "key_bind.h"
#include <scene_loader.h>
#include "debugger.h"

namespace game {
	class gameplay_manager : public scripts_system::script {
	public:
		gameplay_manager();
		void start() override;
		static glm::vec3* player_position;
		static bool game_paused;
		static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
		static void pause();
		static void un_pause();
		static void full_screen();

	private:
		static double _time_scale_buffor;
		static void pause_un_pause();
		input_system::key_bind pause_key_bind = input_system::key_bind([]() { game::gameplay_manager::pause_un_pause(); }, GLFW_KEY_ESCAPE, GLFW_PRESS);

		input_system::key_bind terminal_key_bind = input_system::key_bind([]() { game::debugger::toggle(); }, GLFW_KEY_F3, GLFW_PRESS);

		input_system::key_bind full_screen_bind = input_system::key_bind([]() { game::gameplay_manager::full_screen(); }, GLFW_KEY_F1, GLFW_PRESS);
	};
}
