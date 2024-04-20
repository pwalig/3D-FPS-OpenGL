#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
namespace engine {
	class transform {
	public:
		glm::vec3 local_position;
		glm::quat local_rotation;
		glm::vec3 local_scale;

		transform* parent;

		glm::mat4 global_model_matrix() const;
	};
}