#pragma once
#include "scripts_system.h"
#include "input_system.h"
#include "key_bind.h"
#include <scene_loader.h>
#include "debugger.h"
#include "pause_menu.h"
#include "settings_menu.h"
#include "game_over_menu.h"
#include <graphics_menu.h>

namespace game {
	class gameplay_manager : public scripts_system::script {
	public:
		gameplay_manager();
		static gameplay_manager* instance;
		void start() override;
		static glm::vec3* player_position;
		static bool game_paused;
		static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
		static void pause();
		static void un_pause();
		static void full_screen();
		~gameplay_manager();

		static float difficulty_float; // ranges from 0.01 to 100, =1 -> normal difficulty, >1 -> hard, <1 -> easy
		static float multiply_by_difficulty(const float& value, const float& influence = 1.0f, const bool& inverse = false);
		static int multiply_by_difficulty(const int& value, float influence = 1.0f, const bool& inverse = false);

	private:
		static double _time_scale_buffor;
		static void pause_un_pause();
		input_system::key_bind pause_key_bind = input_system::key_bind([]() {
			if (game::game_over_menu::instance) return; // cannot pause if game over screen is on
			if (game::graphics_menu::instance) {
				game::graphics_menu::instance->back.on_click.call_events(); // close graphics menu
			}
			else if (game::settings_menu::instance) {
				game::settings_menu::instance->back.on_click.call_events(); // close settings menu
			}
			else if (game::pause_menu::instance) {
				game::pause_menu::instance->un_pause.on_click.call_events(); // close pause menu
			}
			else game::gameplay_manager::pause_un_pause();
			}, GLFW_KEY_ESCAPE, GLFW_PRESS);
#ifdef _DEBUG
		input_system::key_bind terminal_key_bind = input_system::key_bind([]() { game::debugger::toggle(); }, GLFW_KEY_F3, GLFW_PRESS);
#endif

		input_system::key_bind full_screen_bind = input_system::key_bind([]() { game::gameplay_manager::full_screen(); }, GLFW_KEY_F1, GLFW_PRESS);
	};
}
