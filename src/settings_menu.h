#pragma once
#include "game_script.h"
#include <ui_slider.h>
#include <ui_text_button.h>
#include <ui_checkbox.h>
#include <ui_dropdown.h>

namespace game {
    class settings_menu : public scripts_system::script {
    public:
        ui_system::ui_text title;
        ui_system::ui_text volume_text;
        ui_system::ui_slider volume;
        ui_system::ui_text difficulty_text;
        ui_system::ui_slider difficulty;
        ui_system::ui_text_button back;
        ui_system::ui_checkbox uic;
        ui_system::ui_dropdown uid;
        settings_menu(const std::function<void()>& on_close = []() {});
        ~settings_menu();

        static game::settings_menu* instance;
    };
}
