#pragma once
#include "game_script.h"
#include <ui_text_button.h>
#include <ui_slider.h>

namespace game {
    class pause_menu : public scripts_system::script {
    public:
        ui_system::ui_text paused;
        ui_system::ui_text_button un_pause;
        ui_system::ui_text_button settings;
        ui_system::ui_text_button main_menu;
        ui_system::ui_text_button quit;
        pause_menu();
        ~pause_menu();

        static game::pause_menu* instance;
    };
}
