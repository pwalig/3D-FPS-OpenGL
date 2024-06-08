#pragma once
#include "game_script.h"
#include "timer.h"
#include "ui_image.h"

namespace game{
	class crosshair_indicator : public scripts_system::script {
	public:
		time_system::function_timer ft;
		ui_system::ui_image uii;
		crosshair_indicator(const std::string& img);

		void update() override;
	};
}

