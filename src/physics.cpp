#include "physics.h"
#include "engine.h"
#include "constants.h"

using namespace physics;

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