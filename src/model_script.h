#pragma once
#include "game_script.h"
#include <model.h>

namespace game {
	class model_script : public scripts_system::script {
	public:
		renderer::model* m;
		model_script(renderer::model* model); // call new renderer::..._model(...) in this constructor
		~model_script();
	};
}

