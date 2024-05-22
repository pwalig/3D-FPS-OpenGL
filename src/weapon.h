#pragma once
#include <glm/glm.hpp>
#include <timer.h>
#include <entity.h>
#include <physics.h>

namespace game {
    class weapon {
    public:
        static void hit_scan(const physics::ray& r, const std::function<void(game::entity*)>& on_hit, const std::function<void()>& on_miss = [](){});

        bool auto_repeat = false;
        float cooldown = 0.2f;
        float recoil = 0.1f;
        std::function<void(game::entity*)> on_hit;
        std::function<void()> on_miss = []() {};
        std::function<void(const glm::vec3& position, const glm::vec3& direction, const int& layer)> shoot;
    };
}

