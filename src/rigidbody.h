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
		float moment_of_inertia = 100.0f; // [kg*m^2]
		float restitution = 0.5f;
		glm::vec3 velocity = glm::vec3(0.0f); // [m/s]
		glm::quat angular_speed = glm::quat(glm::vec3(0, 0, 0));
		glm::vec3 force = glm::vec3(0.0f); // [N]
		glm::vec3 temp_force = glm::vec3(0.0f);
		glm::vec3 torque = glm::vec3(0.0f);
		glm::vec3 temp_torque = glm::vec3(0.0f);

		void update();

		glm::vec3 get_force(const glm::vec3& force, const glm::vec3& position);
		glm::vec3 get_torque(const glm::vec3& force, const glm::vec3& position);
		glm::mat4 model_matrix() const;
	};
}
