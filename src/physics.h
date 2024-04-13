#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace physics {
	const glm::vec3 gravity = glm::vec3(0.0f, -9.81f, 0.0f);

	struct rigidbody {
	public:
		glm::mat4x4 transform = glm::mat4x4(1.0f);
		glm::vec3 position = glm::vec3(0.0f);
		glm::vec3 velocity = glm::vec3(0.0f);
	};

	void update(rigidbody* rb, const glm::vec3 &force, const glm::vec3 &placement = glm::vec3(0.0f));
}