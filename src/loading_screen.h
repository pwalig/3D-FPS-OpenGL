#pragma once
#include "game_script.h"
#include <ui_text.h>
#include <ui_animation.h>

namespace game {
    class loading_screen : public scripts_system::script {
    public:
        loading_screen(const std::string& initial_info_text = "");
        void update_info_text(const std::string& new_progress_text);
    private:
        ui_system::ui_text loading_text;
        ui_system::ui_text info_text;

        std::vector<ui_system::ui_visual*> to_draw;

        void draw();
    };
}
