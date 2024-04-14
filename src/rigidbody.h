#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

namespace physics {

	class rigidbody {
	public:
		bool dynamic = true;
		glm::vec3 position = glm::vec3(0.0f);
		glm::quat rotation = glm::quat(glm::vec3(0, 0, 0));
		float mass = 10.0f; // [kg]
		float moment_of_inertia = 10.0f; // [kg*m^2]
		glm::vec3 velocity = glm::vec3(0.0f); // [m/s]
		glm::quat angular_speed = glm::quat(glm::vec3(0, 0, 0));
		glm::vec3 force = glm::vec3(0.0f); // [N]
		glm::vec3 torque = glm::vec3(0.0f);

		void update();
		glm::mat4 model_matrix() const;
	};
}
