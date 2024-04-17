#include "collision_detection.h"
#include <glm/geometric.hpp>
#include <functional>

namespace physics {
    std::function<void()>** collision_checkers;
}

void physics::init()
{
    collision_checkers = new std::function<void()>*[COLLIDERS_AMOUNT];
    for (int i = 0; i < COLLIDERS_AMOUNT; i++) {
        collision_checkers[i] = new std::function<void()>[COLLIDERS_AMOUNT];
    }

    //collision_checkers[COLLIDERS_AABB][COLLIDERS_AABB] = check_collision;
}

bool physics::check_collision(const colliders::aabb& a, const colliders::aabb& b)
{
    return (
        a.position.x - (a.size.x / 2.0f) <= b.position.x + (b.size.x / 2.0f) &&
        a.position.x + (a.size.x / 2.0f) >= b.position.x - (b.size.x / 2.0f) &&
        a.position.y - (a.size.y / 2.0f) <= b.position.y + (b.size.y / 2.0f) &&
        a.position.y + (a.size.y / 2.0f) >= b.position.y - (b.size.y / 2.0f) &&
        a.position.z - (a.size.z / 2.0f) <= b.position.z + (b.size.z / 2.0f) &&
        a.position.z + (a.size.z / 2.0f) >= b.position.z - (b.size.z / 2.0f)
    );
}

bool physics::check_collision(const colliders::sphere& a, const colliders::sphere& b)
{
    return glm::length(a.position - b.position) <= a.radius + b.radius;
}
