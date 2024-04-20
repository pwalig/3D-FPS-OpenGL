#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
namespace engine {
	class transform {
	public:
		glm::vec3 local_position = glm::vec3(0.0f);
		glm::quat local_rotation = glm::quat(glm::vec3(0.0f));
		glm::vec3 local_scale = glm::vec3(1.0f);

		transform* parent = nullptr;

		glm::mat4 global_model_matrix() const;
	};
}