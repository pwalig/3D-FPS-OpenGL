#pragma once
#include "game_script.h"
#include <ui_image.h>
#include <ui_text.h>
#include "ui_model.h"
#include "timer.h"

namespace game {
	class player_ui : public scripts_system::script {
	public:
		ui_system::ui_image crosshair;
		ui_system::ui_image hp_bar;

		player_ui();
	};
}

