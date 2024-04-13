#include "physics.h"
#include "engine.h"

using namespace physics;

void physics::update(rigidbody* rb, const glm::vec3 &force, const glm::vec3 &placement)
{
	rb->velocity += force * engine::delta_time;
	rb->position += rb->velocity * engine::delta_time;
	rb->transform = glm::translate(glm::mat4x4(1.0f), rb->position);
}
