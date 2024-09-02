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
        ui_system::ui_slider framerate;
        ui_system::ui_text framerate_text;
        ui_system::ui_checkbox framerate_enable;
        ui_system::ui_checkbox vsynch;
        ui_system::ui_text vsynch_text;
        ui_system::ui_checkbox fullscreen;
        ui_system::ui_text fullscreen_text;

        ui_system::ui_slider gamma;
        ui_system::ui_text gamma_text;
        ui_system::ui_slider exposure;
        ui_system::ui_text exposure_text;

        ui_system::ui_dropdown minimization;
        ui_system::ui_text minimization_text;
        ui_system::ui_dropdown magnification;
        ui_system::ui_text magnification_text;
        ui_system::ui_slider anisotropy;
        ui_system::ui_text anisotropy_text;

        ui_system::ui_text_button back;

        graphics_menu(const std::function<void()>& on_close = []() {});
        ~graphics_menu();

        static graphics_menu* instance;
        static bool vsynch_enabled;
        static unsigned int framerate_cap;
        static bool framerate_cap_enable;
    };
}
