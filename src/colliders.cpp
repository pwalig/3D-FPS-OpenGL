#include "colliders.h"

physics::colliders::aabb::aabb(glm::vec3& position_, const glm::vec3& size_) : position(position_), size(size_) {}

physics::colliders::sphere::sphere(glm::vec3& position_, const float& radius_) : position(position_), radius(radius_) {}