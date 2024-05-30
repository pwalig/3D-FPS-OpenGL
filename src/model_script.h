#pragma once
#include "game_script.h"
#include <model.h>
#include <GL/glew.h>
#include <pbr_model.h>
namespace game {
	class model_script : public scripts_system::script {
	public:
		renderer::pbr_model::model* m;
		model_script(const std::string& mesh, const std::string& texture0, const std::string& texture1, const std::string& texture2, const glm::mat4& initial_matrix = glm::mat4(1.0f));
		~model_script();
	};
}

