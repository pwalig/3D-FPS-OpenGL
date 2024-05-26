#pragma once
#include <vector>
#include "mesh.h"
#include <glm/glm.hpp>

namespace renderer {

	class model {
	public:
		glm::mat4 model_matrix;  // model matrix as a regular variable
		mesh_ptr mesh;

		model(const glm::mat4& initialMatrix = glm::mat4(1.0f));
		model(const std::string& mesh_, const glm::mat4& initialMatrix = glm::mat4(1.0f));
		virtual void draw() {};
		virtual ~model();

		static void draw_all_models();

	private:
		static std::vector<renderer::model*> all_models; //vector of all of the models
	};
}

