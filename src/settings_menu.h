#pragma once
#include "game_script.h"
#include <ui_slider.h>
#include <ui_text_button.h>
#include <ui_checkbox.h>
#include <ui_dropdown.h>
#include <ui_input_text.h>

namespace game {
    class settings_menu : public scripts_system::script {
    public:
        ui_system::ui_text title;
        ui_system::ui_text volume_text;
        ui_system::ui_slider volume;
        ui_system::ui_text difficulty_text;
        ui_system::ui_slider difficulty;
        ui_system::ui_text mouse_sensitivity_text;
        ui_system::ui_slider mouse_sensitivity;
        ui_system::ui_text_button graphics;
        ui_system::ui_text_button back;
        settings_menu();
        ~settings_menu();

        static game::settings_menu* instance;
    };
}
