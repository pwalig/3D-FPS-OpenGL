#pragma once
#include "game_script.h"
#include <model.h>
#include <GL/glew.h>
#include <lambert_textured_model.h>
namespace game {
	class model_script : public scripts_system::script {
	public:
		renderer::lambert_textured_model::model* m;
		model_script(const std::string& mesh, const GLuint* texture, const glm::mat4& initial_matrix = glm::mat4(1.0f));
		~model_script();
	};
}

