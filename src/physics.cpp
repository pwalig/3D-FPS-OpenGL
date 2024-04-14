#include "physics.h"
#include "engine.h"
#include "constants.h"

using namespace physics;

void physics::update(rigidbody* rb)
{
	rb->velocity += rb->force * (float)engine::delta_time / rb->mass;
	rb->position += rb->velocity * (float)engine::delta_time;
	rb->transform = glm::translate(glm::mat4x4(1.0f), rb->position);

	if (glm::length(rb->torque) != 0.0f) rb->angular_speed = glm::rotate(rb->angular_speed, glm::length(rb->torque) * (float)engine::delta_time / rb->moment_of_inertia, glm::normalize(rb->torque));
	rb->rotation *= rb->angular_speed;
	rb->transform *= rb->rotation;
}


glm::vec3 physics::get_force(const glm::vec3& force, const glm::vec3& arm) {
	return glm::normalize(-arm) * glm::dot(force, glm::normalize(-arm));
}

glm::vec3 physics::get_torque(const glm::vec3& force, const glm::vec3& arm) {
	return glm::cross(force, arm);
}

void physics::collide(rigidbody* rb1, const float& restitution)
{
	if (rb1->dynamic) {
		rb1->velocity = -rb1->velocity * restitution;
	}
}