#pragma once
#include "game_script.h"
#include <model.h>
namespace game {
	class model_script : public scripts_system::script {
	public:
		renderer::model* m;
		model_script(const std::string& mesh, const glm::mat4& initial_matrix = glm::mat4(1.0f));
		~model_script();
	};
}

