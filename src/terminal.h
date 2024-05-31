#pragma once
#include "game_script.h"
#include <key_bind.h>
#include <ui_text.h>
#include <vector>
#include <map>
#include "timer.h"

namespace game {
    class terminal : public scripts_system::script {
    private:
        static terminal* active_terminal;
        static std::map<std::string, std::function<void(std::vector<std::string>)>> commands;

        std::vector<int*> subscribes;
    public:
        static void terminal_toggle();


        int frames = 0;
        time_system::function_timer ft;
        ui_system::ui_text fps_meter;

        ui_system::ui_text input_text;
        ui_system::ui_text info_text;
        terminal();
        void update() override;
        void process_command();
    };
}

