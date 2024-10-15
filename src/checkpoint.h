#pragma once
#include "game_script.h"
#include <glm/glm.hpp>
#include "physics.h"
#include <key_bind.h>
#include "light.h"
#include "ui_text.h"

namespace game {
	class checkpoint : public scripts_system::script {
	public:
		checkpoint(const glm::vec3& position, const float& radius);

		glm::vec3 position;
		physics::colliders::sphere col;
		renderer::light li;

	private:
		input_system::key_bind* kb;
		ui_system::ui_text* txt;
		void open_menu();
	};
}
