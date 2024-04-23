#pragma once
#include <physics.h>
#include <scripts_system.h>
#include <input_system.h>
#include <axis.h>
#include <constants.h>
#include <key_bind.h>

namespace game {
	class player : public scripts_system::script {
	public:
		player(const glm::vec3& initial_position, const float& y_rotation);
		void update() override;

	private:
		void jump();
		void land(physics::collision_info ci);

		physics::rigidbody rb;
		physics::colliders::sphere col;

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
		input_system::key_bind reset_key_bind = input_system::key_bind([&, this]() {this->rb.position = glm::vec3(0.0f, 2.0f, 0.0f); }, GLFW_KEY_R, GLFW_PRESS); // temporary function for reseting player position
	};
}