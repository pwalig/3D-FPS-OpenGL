#pragma once
#include "game_script.h"
#include <ui_visual.h>
namespace game {
	class player_ui : public scripts_system::script {
	public:
		ui_system::ui_visual crosshair;
		ui_system::ui_visual hp_bar;

		player_ui();
	};
}

