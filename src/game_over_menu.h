#pragma once
#include "game_script.h"
#include <ui_text.h>
#include <input_system.h>
#include "key_bind.h"

namespace game {
    class game_over_menu : public scripts_system::script {
    public:
        game_over_menu();
        ui_system::ui_text uit;
        void restart();
        input_system::key_bind restart_key_bind = input_system::key_bind(std::bind(&game::game_over_menu::restart, this), GLFW_KEY_ENTER, GLFW_PRESS);
    };
}
