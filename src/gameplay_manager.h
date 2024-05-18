#pragma once
#include "scripts_system.h"
#include "input_system.h"
#include "key_bind.h"
#include <scene_loader.h>

namespace game {
	class gameplay_manager : public scripts_system::script {
	public:
		gameplay_manager();
		void start() override;
		static glm::vec3* player_position;
	private:
		bool game_paused;
		double _time_scale_buffor;
		void pause_un_pause();
		void pause();
		void un_pause();

		input_system::key_bind pause_key_bind = input_system::key_bind(std::bind(&game::gameplay_manager::pause_un_pause, this), GLFW_KEY_ESCAPE, GLFW_PRESS);

		input_system::key_bind scene_load_key_bind = input_system::key_bind(std::bind(scene_loader::load_scene, "example_scene2.json"), GLFW_KEY_L, GLFW_PRESS);
		input_system::key_bind scene_un_load_key_bind = input_system::key_bind(std::bind(scene_loader::un_load_scene, "example_scene3.json"), GLFW_KEY_L, GLFW_PRESS);
		// scene loading at key action only possible if object will not be removed as a result of un_load
	};
}