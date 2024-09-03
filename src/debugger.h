#pragma once
#include "game_script.h"
#include <key_bind.h>
#include <ui_text.h>
#include <vector>
#include <map>
#include "timer.h"
#include <GLFW/glfw3.h>

#ifdef _DEBUG

namespace game {
    class debugger : public scripts_system::script {
    private:
        static debugger* active_debugger;
        static std::map<std::string, std::function<void(std::vector<std::string>)>> commands;

        int frames = 0;
        time_system::function_timer ft;

        void toggle_terminal();
        void process_command();

    public:
        static void toggle();
        static float scripts_time;
        static float timers_time;
        static float physics_time;
        static float render_time;
        static float callbacks_time;

        ui_system::ui_text fps_meter;

        ui_system::ui_text input_text;
        ui_system::ui_text info_text;
        debugger();
        void update() override;

    private:
        input_system::key_bind tt_key_bind = input_system::key_bind(std::bind(&game::debugger::toggle_terminal, this), GLFW_KEY_SLASH, GLFW_PRESS);
        input_system::key_bind pc_key_bind = input_system::key_bind(std::bind(&game::debugger::process_command, this), GLFW_KEY_ENTER, GLFW_PRESS);
    };
}

#endif
