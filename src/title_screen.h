#pragma once
#include "game_script.h"
#include "ui_text.h"
#include <key_bind.h>

namespace game {
	class title_screen : public scripts_system::script {
	public:
		ui_system::ui_text text;

		title_screen();
		~title_screen();

		static title_screen* instance;
		input_system::key_bind proceed_key_bind;
	};
}
