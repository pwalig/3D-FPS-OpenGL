#pragma once
#include <glm/ext/vector_float3.hpp>

#define COLLIDERS_AABB 0
#define COLLIDERS_SPHERE 1
#define COLLIDERS_CUBE 2
#define COLLIDERS_CAPSULE 3
#define COLLIDERS_AMOUNT 2

namespace physics {
	namespace colliders {
		struct aabb {
			glm::vec3& position;
			glm::vec3 size;
			aabb(glm::vec3& position_, const glm::vec3& size_);
		};

		struct sphere {
			glm::vec3& position;
			float radius;
			sphere(glm::vec3& position_, const float& radius_);
		};
	}
}