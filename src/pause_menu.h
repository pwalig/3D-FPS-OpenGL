#pragma once
#include "game_script.h"
#include <ui_vbutton.h>
#include <ui_slider.h>

namespace game {
    class pause_menu : public scripts_system::script {
    public:
        ui_system::ui_slider uis;
        ui_system::ui_text paused;
        ui_system::ui_vbutton un_pause;
        ui_system::ui_vbutton settings;
        ui_system::ui_vbutton main_menu;
        ui_system::ui_vbutton quit;
        pause_menu();
        ~pause_menu();

        static game::pause_menu* instance;
    };
}
