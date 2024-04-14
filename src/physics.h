#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "rigidbody.h"

namespace physics {
	const glm::vec3 gravity = glm::vec3(0.0f, -9.81f, 0.0f);

	glm::vec3 get_force(const glm::vec3& force, const glm::vec3& arm);
	glm::vec3 get_torque(const glm::vec3& force, const glm::vec3& arm);
	void collide(rigidbody* rb1, const float &restitution);
}