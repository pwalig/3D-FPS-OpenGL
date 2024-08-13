#pragma once
#include <glm/glm.hpp>
#include <timer.h>
#include <entity.h>
#include <physics.h>
#include <map>

namespace game {
    class weapon {
    public:
        static void hit_scan(const physics::ray& r, const std::function<void(game::entity*)>& on_hit, const std::function<void()>& on_miss = [](){});
        static std::map<std::string, game::weapon*> weapon_map;
        static void init();
        static void free();

        bool auto_repeat = false;
        float cooldown = 0.2f;
        float recoil = 0.5f;
        float scope = 0.5f;
        std::function<void(const glm::vec3& position, const glm::vec3& direction, const int& layer)> shoot;
    };

    namespace weapons {
        extern weapon* throwable_cube;
    }
}
