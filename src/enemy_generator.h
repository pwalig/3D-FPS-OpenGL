#pragma once
#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>
#include <timer.h>


namespace scene_loader {
    namespace generator {
        extern time_system::function_timer* generate_enemy_cooldown;
        void initialize_enemies(const std::string& scene_name);
        void schedule_enemy_initialization(const std::string& scene_name);
        void init();
        extern float period;
        void reset_period();
    }
}
