#pragma once
#include "game_script.h"
#include <ui_image.h>
#include <ui_text.h>
#include "timer.h"

namespace game {
	class player_ui : public scripts_system::script {
	public:
		ui_system::ui_image crosshair;
		ui_system::ui_image hp_bar;

		int frames = 0;
		time_system::function_timer t;
		ui_system::ui_text fps_meter;

		player_ui();
		void update() override;
	};
}

