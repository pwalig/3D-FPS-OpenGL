#pragma once
#include "game_script.h"
#include <ui_text_button.h>
#include "ui_draggable.h"
#include "player_script.h"

namespace game {
    class checkpoint_menu : public scripts_system::script {
    public:
        struct slot {
            glm::vec3 position;
            ui_system::ui_draggable* occupant;
            ui_system::ui_image image;

            slot(const glm::vec3& position_);
            slot(ui_system::ui_draggable* draggable);
            void drop_cube(ui_system::ui_draggable* cube);
        };
        struct cube {
            game::power_cube* ptr;
            slot* slot_ptr;
            ui_system::ui_draggable* draggable;
        };
        ui_system::ui_text title;
        ui_system::ui_text text;
        ui_system::ui_text_button back;
        std::vector<cube> cubes;
        std::vector<slot> cube_slots;
        game::player* served_player = nullptr;
        checkpoint_menu(game::player* player_to_serve);
        ~checkpoint_menu();

        static game::checkpoint_menu* instance;
        slot* get_closest_slot(const glm::vec3& position);
        void setup_slots();
        void add_cube_and_slot(game::power_cube* cube, const glm::vec3& position);

        void pick_up_cube(const ui_system::ui_draggable* cube);

        void drop_cube(ui_system::ui_draggable* cube);
        slot& get_empty_slot();
    };
}

