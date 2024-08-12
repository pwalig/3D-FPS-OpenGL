#pragma once
#include <functional>
#include <ui_model.h>
#include "timer.h"

namespace game {
	class player;
}
#include "player_script.h"

namespace game {
	class power_cube : public scripts_system::script {
	public:
		char type = 'a';
		float cooldown = 3.0f;
		int healing = 25;
		time_system::function_timer t;
		std::function<void()> on_use = [this]() {
			printf("used\n");
			};
		void use();

		power_cube(game::player* owner);
		void update() override;

		glm::vec3 target_ui_pos = glm::vec3(0.0f);
		void set_ui_position(const glm::vec3& new_position);
		friend player;

	private:
		ui_system::ui_model visual;
		physics::rigidbody visual_rb;
	};
}
