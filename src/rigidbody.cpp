#include "rigidbody.h"
#include "physics.h"
#include <engine.h>
#include <time_system.h>

physics::rigidbody::rigidbody()
{
	physics::rigidbodies.push_back(this); // register collider for physics updates
}

void physics::rigidbody::update()
{
	if (dynamic) {
		this->velocity += (this->force + this->temp_force) * (float)time_system::delta_time / this->mass;
		this->position += this->velocity * (float)time_system::delta_time;

		this->temp_torque += this->torque;
		if (glm::length(this->temp_torque) != 0.0f) {
			this->angular_speed = glm::rotate(this->angular_speed, glm::length(this->temp_torque) * (float)time_system::delta_time / this->moment_of_inertia, glm::normalize(this->temp_torque));
			this->angular_speed = glm::normalize(this->angular_speed);
		}
		this->rotation *= this->angular_speed;
		this->temp_force = glm::vec3(0.0f);
		this->temp_torque = glm::vec3(0.0f);
	}
}

glm::vec3 physics::rigidbody::get_force(const glm::vec3& force_, const glm::vec3& position_)
{
	glm::vec3 arm = this->position - position_;
	return glm::normalize(arm) * glm::dot(-force, glm::normalize(arm));
}

glm::vec3 physics::rigidbody::get_torque(const glm::vec3& force_, const glm::vec3& position_)
{
	glm::vec3 arm = position_ - this->position;
	return glm::cross(force, arm);
}

glm::mat4 physics::rigidbody::model_matrix() const
{
	// calculate model matrix
	glm::mat4 M = glm::translate(glm::mat4(1.0f), this->position);
	M *= glm::toMat4(this->rotation);
	return M;
}

physics::rigidbody::~rigidbody()
{
	std::vector<physics::rigidbody*>::iterator id = std::find(physics::rigidbodies.begin(), physics::rigidbodies.end(), this);
	if (id != physics::rigidbodies.end()) physics::rigidbodies.erase(id); // remove rigidbody from physics::rigidbodies so that it is not called when destroyed
}
