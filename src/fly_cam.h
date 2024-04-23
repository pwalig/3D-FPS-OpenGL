#pragma once
#include "scripts_system.h"
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <input_system.h>
#include <axis.h>
#include "constants.h"

namespace game {
	class fly_cam : public scripts_system::script {
	public:
		void start() override;
		void update() override;
	private:
		glm::vec3 position = glm::vec3(0.0f);
		glm::quat rotation = glm::quat(glm::vec3(0.0f));

		float speed = 0.2f;

		float rot_speed = PI * 0.05f;
		float max_rot = PI * 2.3f;
		glm::vec2 rot2 = glm::vec2(0.0f);

		input_system::triple_axis move_in = input_system::triple_axis(GLFW_KEY_A, GLFW_KEY_D, GLFW_KEY_E, GLFW_KEY_Q, GLFW_KEY_W, GLFW_KEY_S);
		input_system::double_axis rot_in = input_system::double_axis(GLFW_KEY_DOWN, GLFW_KEY_UP, GLFW_KEY_LEFT, GLFW_KEY_RIGHT);
	};
}