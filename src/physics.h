#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace physics {
	const glm::vec3 gravity = glm::vec3(0.0f, -9.81f, 0.0f);

	struct rigidbody {
	public:
		bool dynamic = true;
		glm::vec3 position = glm::vec3(0.0f);
		glm::mat4x4 rotation = glm::mat4x4(1.0f);
		float mass = 1.0f;
		float moment_of_inertia = 10.0f;
		glm::vec3 velocity = glm::vec3(0.0f);
		glm::mat4x4 angular_speed = glm::mat4x4(1.0f);
		glm::vec3 force = glm::vec3(0.0f);
		glm::vec3 torque = glm::vec3(0.0f);
		glm::mat4x4 transform = glm::mat4x4(1.0f);
	};

	void update(rigidbody* rb);

	glm::vec3 get_force(const glm::vec3& force, const glm::vec3& arm);
	glm::vec3 get_torque(const glm::vec3& force, const glm::vec3& arm);
	void collide(rigidbody* rb1, const float &restitution);
}