#pragma once
#include "game_script.h"
#include <glm/glm.hpp>
#include <timer.h>
#include <entity.h>
#include <physics.h>

namespace game {
    class weapon : public scripts_system::script {
    public:
        static void hit_scan(const physics::ray& r, const std::function<void(game::entity*)>& on_hit, const std::function<void()>& on_miss = [](){});
        bool auto_repeat = false;
        float cooldown = 0.2f;
        float recoil = 0.1f;
        virtual void shoot(const glm::vec3& position, const glm::vec3& direction, const int& layer);
    };
}

