#include "rigidbody.h"
#include <engine.h>
#include <time_system.h>

void physics::rigidbody::update()
{
	if (dynamic) {
		this->velocity += this->force * (float)time::delta_time / this->mass;
		this->position += this->velocity * (float)time::delta_time;

		if (glm::length(this->torque) != 0.0f) {
			this->angular_speed = glm::rotate(this->angular_speed, glm::length(this->torque) * (float)time::delta_time / this->moment_of_inertia, glm::normalize(this->torque));
			this->angular_speed = glm::normalize(this->angular_speed);
		}
		this->rotation *= this->angular_speed;
	}
}

glm::mat4 physics::rigidbody::model_matrix() const
{
	// calculate model matrix
	glm::mat4 M = glm::translate(glm::mat4(1.0f), this->position);
	M *= glm::toMat4(this->rotation);
	return M;
}
