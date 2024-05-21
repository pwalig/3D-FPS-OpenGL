#pragma once
#include "game_script.h"
#include <ui_image.h>
#include <ui_text.h>
namespace game {
	class player_ui : public scripts_system::script {
	public:
		ui_system::ui_image crosshair;
		ui_system::ui_image hp_bar;

		ui_system::ui_text test_text;

		player_ui();
	};
}

