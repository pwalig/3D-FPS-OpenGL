#include "collision_detection.h"
#include <glm/geometric.hpp>
#include <functional>
#include <vector>

namespace physics {
    std::function<void()>** collision_checkers;
    std::vector<collider*> all_colliders;
}


// COLLIDERS

physics::colliders::aabb::aabb(glm::vec3& position_, const glm::vec3& size_) : collider(nullptr), position(position_), size(size_) {}

int physics::colliders::aabb::get_type() { return COLLIDERS_AABB; }

physics::colliders::sphere::sphere(glm::vec3& position_, const float& radius_) : collider(nullptr), position(position_), radius(radius_) {}

int physics::colliders::sphere::get_type() { return COLLIDERS_SPHERE; }

physics::colliders::plane::plane(glm::vec3& position_, glm::quat& rotation_, const glm::vec2& size_) : collider(nullptr), position(position_), rotation(rotation_), size(size_) {}

int physics::colliders::plane::get_type() { return COLLIDERS_PLANE; }

physics::collider::collider() : rigidbody(nullptr) {}

physics::collider::collider(physics::rigidbody* const rb) : rigidbody(rb) {}

void physics::collider::collision_notify(const physics::collision_info& ci)
{
    if (std::find(this->_collided_last_frame.begin(), this->_collided_last_frame.end(), ci.other) != this->_collided_last_frame.end()) {
        on_collision_stay.call_events(ci);
    }
    else {
        on_collision_enter.call_events(ci);
    }
    this->_collided_this_frame.push_back(ci.other);
}

void physics::collider::swap_collider_buffers()
{
    for (auto& c : _collided_last_frame) {
        if (std::find(this->_collided_this_frame.begin(), this->_collided_this_frame.end(), c) == this->_collided_this_frame.end()) {
            on_collision_exit.call_events();
        }
    }
    //std::copy(this->_collided_this_frame.begin(), this->_collided_this_frame.end(), this->_collided_last_frame);
    this->_collided_last_frame = this->_collided_this_frame;
    this->_collided_this_frame.clear();
}

int physics::collider::get_type() { return COLLIDERS_NONE; }


// COLLISION DETECTION

void physics::detect_collisions()
{
    for (std::vector<collider*>::iterator it1 = all_colliders.begin(); it1 != all_colliders.end(); ++it1) {
        for (std::vector<collider*>::iterator it2 = it1 + 1; it2 != all_colliders.end(); ++it2) {
            collider* c1 = (*it1);
            collider* c2 = (*it2);
            switch (c1->get_type())
            {
            case COLLIDERS_AABB:
                switch (c2->get_type())
                {
                case COLLIDERS_AABB:
                    check_collision<colliders::aabb, colliders::aabb>(c1, c2);
                    break;
                case COLLIDERS_SPHERE:
                    //check_collision<colliders::aabb, colliders::sphere>(c1, c2);
                    break;
                case COLLIDERS_PLANE:
                    //check_collision<colliders::aabb, colliders::plane>(c1, c2);
                    break;
                default:
                    break;
                }
                break;
            case COLLIDERS_SPHERE:
                switch (c2->get_type())
                {
                case COLLIDERS_AABB:
                    //check_collision<colliders::sphere, colliders::aabb>(c1, c2);
                    break;
                case COLLIDERS_SPHERE:
                    check_collision<colliders::sphere, colliders::sphere>(c1, c2);
                    break;
                case COLLIDERS_PLANE:
                    check_collision<colliders::sphere, colliders::plane>(c1, c2);
                    break;
                default:
                    break;
                }
                break;
            case COLLIDERS_PLANE:
                switch (c2->get_type())
                {
                case COLLIDERS_AABB:
                    //check_collision<colliders::plane, colliders::aabb>(c1, c2);
                    break;
                case COLLIDERS_SPHERE:
                    check_collision<colliders::plane, colliders::sphere>(c1, c2);
                    break;
                case COLLIDERS_PLANE:
                    check_collision<colliders::plane, colliders::plane>(c1, c2);
                    break;
                default:
                    break;
                }
                break;
            default:
                // to do: cube
                break;
            }
        }
    }
    for (auto c : all_colliders) {
        c->swap_collider_buffers();
    }
}

physics::collision_info physics::get_collision_info(const colliders::aabb& a, const colliders::aabb& b)
{
    collision_info ci;
    ci.collision = (
        a.position.x - (a.size.x / 2.0f) <= b.position.x + (b.size.x / 2.0f) &&
        a.position.x + (a.size.x / 2.0f) >= b.position.x - (b.size.x / 2.0f) &&
        a.position.y - (a.size.y / 2.0f) <= b.position.y + (b.size.y / 2.0f) &&
        a.position.y + (a.size.y / 2.0f) >= b.position.y - (b.size.y / 2.0f) &&
        a.position.z - (a.size.z / 2.0f) <= b.position.z + (b.size.z / 2.0f) &&
        a.position.z + (a.size.z / 2.0f) >= b.position.z - (b.size.z / 2.0f)
    );
    return ci;
}

physics::collision_info physics::get_collision_info(const colliders::sphere& a, const colliders::sphere& b)
{
    collision_info ci;
    ci.collision = glm::length(a.position - b.position) <= a.radius + b.radius;
    if (ci.collision) {
        ci.collision_point = ((a.position * a.radius) + (b.position * b.radius)) / (a.radius + b.radius);
        ci.normal = glm::normalize(b.position - a.position);
    }
    return ci;
}

physics::collision_info physics::get_collision_info(const colliders::sphere& s, const colliders::plane& p)
{
    collision_info ci;
    glm::vec3 srp = glm::inverse(p.rotation) * (s.position - p.position); // sphere relative position
    float intersect = s.radius - srp.z;
    ci.collision = (
        srp.x <= p.size.x / 2.0f &&
        srp.x >= -p.size.x / 2.0f &&
        srp.y <= p.size.y / 2.0f &&
        srp.y >= -p.size.y / 2.0f &&
        intersect >= 0.0f
    );
    if (ci.collision) {
        ci.normal = glm::vec3(0.0f, 1.0f, 0.0f) * p.rotation;
        ci.collision_point = p.rotation * (srp + p.position - s.position) - ci.normal * (s.radius - (intersect / 2.0f));
    }
    return ci;
}

physics::collision_info physics::get_collision_info(const colliders::plane& p, const colliders::sphere& s)
{
    return get_collision_info(s, p);
}

physics::collision_info physics::get_collision_info(const colliders::plane& a, const colliders::plane& b)
{
    collision_info ci;
    ci.collision = false;
    return ci;
}
