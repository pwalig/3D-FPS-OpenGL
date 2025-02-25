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
	if (dynamic && time_system::delta_time < 0.1) { // if game had longer frame (loading time) dont update
		this->velocity += (this->force + this->temp_force - (this->velocity * this->movement_drag)) * (float)time_system::delta_time / this->mass;
		this->position += this->velocity * (float)time_system::delta_time;

		this->angular_speed += (this->torque + this->temp_torque - (this->angular_speed * this->angular_drag));
		if (glm::length(this->angular_speed) != 0.0f) {
			this->rotation = glm::rotate(this->rotation, glm::length(this->angular_speed) * (float)time_system::delta_time / this->moment_of_inertia, glm::normalize(this->angular_speed));
			this->rotation = glm::normalize(this->rotation);
		}
		this->temp_force = glm::vec3(0.0f);
		this->temp_torque = glm::vec3(0.0f);
	}
}

glm::vec3 physics::rigidbody::get_force(const glm::vec3& force_, const glm::vec3& position_)
{
	glm::vec3 arm = this->position - position_;
	return glm::normalize(arm) * glm::dot(-force_, glm::normalize(arm));
}

glm::vec3 physics::rigidbody::get_torque(const glm::vec3& force_, const glm::vec3& position_)
{
	glm::vec3 arm = position_ - this->position;
	return glm::cross(force_, arm);
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
