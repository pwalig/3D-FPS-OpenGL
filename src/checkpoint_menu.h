#pragma once
#include "game_script.h"
#include <ui_text_button.h>

namespace game {
    class checkpoint_menu : public scripts_system::script {
    public:
        ui_system::ui_text title;
        ui_system::ui_text text;
        ui_system::ui_text_button back;
        checkpoint_menu();
        ~checkpoint_menu();

        static game::checkpoint_menu* instance;
    };
}

