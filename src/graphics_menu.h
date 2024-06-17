#pragma once
#include "game_script.h"
#include <ui_checkbox.h>
#include <ui_dropdown.h>
#include <ui_slider.h>

namespace game {
    class graphics_menu : public scripts_system::script {
    public:
        ui_system::ui_text title;

        ui_system::ui_slider fov;
        ui_system::ui_text fov_text;
        ui_system::ui_checkbox vsynch;
        ui_system::ui_text vsynch_text;

        ui_system::ui_dropdown minimization;
        ui_system::ui_text minimization_text;
        ui_system::ui_dropdown magnification;
        ui_system::ui_text magnification_text;
        ui_system::ui_slider anisotropy;
        ui_system::ui_text anisotropy_text;

        ui_system::ui_text_button back;

        graphics_menu();
        ~graphics_menu();

        static graphics_menu* instance;
        static bool vsynch_enabled;
    };
}
