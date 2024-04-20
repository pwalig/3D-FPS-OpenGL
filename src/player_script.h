#pragma once
#include <physics.h>
#include <scripts_system.h>
#include <input_system.h>
#include <axis.h>
#include <constants.h>

namespace game {
	class player : public scripts_system::script {
	public:
		player();
		void start() override;
		void update() override;

	private:
		physics::rigidbody rb;
		physics::colliders::sphere col;

		// temporary - make floor for player
		physics::rigidbody rb_floor;
		physics::colliders::plane col_floor = physics::colliders::plane(&rb_floor, glm::vec2(100.0f, 100.0f));

		float max_speed = 7.0f; // [m/s]
		float responsiveness = 1.0f; // [m/s^2]
		float ground_responsiveness = 100.0f; // [m/s^2]
		float air_responsiveness = 20.0f; // [m/s^2]

		float jump_force = 5.5f; // actually velocity [m/s]
		bool ready_to_jump = false;

		float rot_speed = PI * 0.5f;
		float max_rot = PI * 0.49f;
		glm::vec2 rot = glm::vec2(0.0f);

		input_system::double_axis move_in = input_system::double_axis(GLFW_KEY_A, GLFW_KEY_D, GLFW_KEY_W, GLFW_KEY_S);

		void jump();
		void land(physics::collision_info ci);
	};
}