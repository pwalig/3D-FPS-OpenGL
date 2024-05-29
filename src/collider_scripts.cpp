#include "collider_scripts.h"


game::colliders::base::base(const glm::vec3& position, const glm::quat& rotation)
{
	rb.position = position;
	rb.rotation = rotation;
	rb.dynamic = false;
}

game::colliders::sphere::sphere(const glm::vec3& position, const float& radius) : base(position), col(&rb, radius)
{
}

game::colliders::aabb::aabb(const glm::vec3& position, const glm::vec3& size) : base(position), col(&rb, size)
{
}

game::colliders::box::box(const glm::vec3& position, const glm::quat& rotation, const glm::vec3& size) : base(position, rotation), col(&rb, size)
{
}

game::colliders::capsule::capsule(const glm::vec3& position, const glm::quat& rotation, const float& radius, const float& spread) : base(position, rotation), col(&rb, radius, spread)
{
}

game::colliders::plane::plane(const glm::vec3& position, const glm::quat& rotation, const glm::vec3& size) : base(position, rotation), col(&rb, size)
{
}
