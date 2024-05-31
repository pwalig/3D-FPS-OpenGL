#pragma once
#include "scripts_system.h"
#include "input_system.h"
#include "key_bind.h"
#include <scene_loader.h>
#include <model_script.h>
#include <textures.h>
#include <renderer.h>
#include "terminal.h"

namespace game {
	class gameplay_manager : public scripts_system::script {
	public:
		gameplay_manager();
		void start() override;
		static glm::vec3* player_position;
		static bool game_paused;
	private:
		game::model_script ms;
		double _time_scale_buffor;
		void pause_un_pause();
		void pause();
		void un_pause();
		void full_screen();

		input_system::key_bind pause_key_bind = input_system::key_bind(std::bind(&game::gameplay_manager::pause_un_pause, this), GLFW_KEY_ESCAPE, GLFW_PRESS);

		input_system::key_bind terminal_key_bind = input_system::key_bind([this]() { game::terminal::terminal_toggle(); }, GLFW_KEY_DELETE, GLFW_PRESS);

		input_system::key_bind full_screen_bind = input_system::key_bind(std::bind(&game::gameplay_manager::full_screen, this), GLFW_KEY_F1, GLFW_PRESS);
	};

}
