#pragma once
#include "game_script.h"
#include <ui_text_button.h>

namespace game {
	class main_menu : public scripts_system::script {
	public:
        ui_system::ui_text_button new_game;
        ui_system::ui_text_button continue_;
        ui_system::ui_text_button load_game;
        ui_system::ui_text_button settings;
        ui_system::ui_text_button quit;

        static main_menu* instance;

        main_menu();
        ~main_menu();
	};
}
