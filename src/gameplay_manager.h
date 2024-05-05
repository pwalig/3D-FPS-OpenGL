#pragma once
#include "scripts_system.h"
#include "input_system.h"
#include "key_bind.h"
#include<timer.h>
#include <scene_loader.h>

namespace game {
	class gameplay_manager : public scripts_system::script {
	public:
		gameplay_manager();
	private:
		bool game_paused;
		double _time_scale_buffor;
		void pause_un_pause();
		void pause();
		void un_pause();

		input_system::key_bind pause_key_bind = input_system::key_bind(std::bind(&game::gameplay_manager::pause_un_pause, this), GLFW_KEY_ESCAPE, GLFW_PRESS);

		input_system::key_bind scene_load_key_bind = input_system::key_bind(std::bind(scene_loader::load_scene, "example_scene2.json"), GLFW_KEY_L, GLFW_PRESS);
	};
}