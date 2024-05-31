#pragma once
#include "game_script.h"
#include <timer.h>
#include <ui_text.h>
#include <rigidbody.h>

namespace game {
	class damage_number : public scripts_system::script {
	public:
		time_system::function_timer ft;
		physics::rigidbody rb;
		ui_system::ui_text uit;
		damage_number(const int& num);

		void update() override;
	};
}

