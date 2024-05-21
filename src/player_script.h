#pragma once
#include <physics.h>
#include <scripts_system.h>
#include <input_system.h>
#include <axis.h>
#include <constants.h>
#include <key_bind.h>
#include "entity.h"

namespace game {
	class player : public game::entity {
	public:
		player(const glm::vec3& initial_position, const float& y_rotation);
		void start() override;
		void update() override;
		void damage(int damage) override;
		void die() override;

	private:
		void jump();
		void land(physics::collision_info ci);
		void shoot();
		void shoot_ray();

		physics::rigidbody rb;
		physics::colliders::sphere col;

		glm::vec3 dir; // looking direction

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
		input_system::key_bind jump_key_bind = input_system::key_bind(std::bind(&game::player::jump, this), GLFW_KEY_SPACE, GLFW_PRESS);
		input_system::key_bind shoot_key_bind = input_system::key_bind(std::bind(&game::player::shoot_ray, this), GLFW_MOUSE_BUTTON_1, GLFW_PRESS);
		input_system::key_bind reset_key_bind = input_system::key_bind([&, this]() {
			this->rb.position = glm::vec3(0.0f, 2.0f, 0.0f);
			this->rb.velocity = glm::vec3(0.0f);
		}, GLFW_KEY_R, GLFW_PRESS); // temporary function for reseting player position
	};
}