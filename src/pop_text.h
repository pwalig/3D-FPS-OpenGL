#pragma once
#include "game_script.h"
#include "timer.h"
#include "rigidbody.h"
#include "ui_text.h"

namespace game {
	class pop_text : public scripts_system::script {
	public:
		time_system::function_timer ft;
		physics::rigidbody rb;
		ui_system::ui_text uit;
		pop_text(const std::string& text, const glm::vec3& position = glm::vec3(0.5f, 0.5f, 0.49f));

		void update() override;
	};
}
