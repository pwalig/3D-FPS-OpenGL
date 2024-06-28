#pragma once
#include "game_script.h"
#include <ui_text_button.h>

namespace game {
    class game_over_menu : public scripts_system::script {
    public:
        ui_system::ui_text title;
        ui_system::ui_text_button retry;
        ui_system::ui_text_button settings;
        ui_system::ui_text_button main_menu;
        ui_system::ui_text_button quit;
        game_over_menu();
        ~game_over_menu();

        static game::game_over_menu* instance;
    };
}
