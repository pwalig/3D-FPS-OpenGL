#include "physics.h"
#include <glm/geometric.hpp>
#include <functional>
#include <vector>
#include "engine.h"
#include "constants.h"

std::vector<physics::collider*> physics::all_colliders;
std::vector<physics::rigidbody*> physics::rigidbodies;

void physics::collide(rigidbody* rb1, rigidbody* rb2, const physics::collision_info& ci)
{
    if (ci.enter_stay) {
        float restitution = (rb1->restitution + rb2->restitution) / 2.0f;
        if (rb1->dynamic && rb2->dynamic) {
            float u1 = glm::dot(-rb1->velocity, ci.normal); // velocity of rb1 along the normal (positive when approaching point of contact)
            float u2 = glm::dot(-rb2->velocity, ci.normal); // velocity of rb2 along the normal (negative when approaching point of contact)
            rb1->velocity += ci.normal * u1; // set rb1 along the normal velocity to 0
            rb2->velocity += ci.normal * u2; // set rb2 along the normal velocity to 0
            float v1 = ((rb1->mass * u1) + (rb2->mass * u2) + (rb2->mass * restitution * (u2 - u1))) / (rb1->mass + rb2->mass); // final velocity of rb1 along the normal
            float v2 = ((rb1->mass * u1) + (rb2->mass * u2) + (rb1->mass * restitution * (u1 - u2))) / (rb1->mass + rb2->mass); // final velocity of rb2 along the normal
            rb1->velocity -= ci.normal * v1; // set rb1 along the normal velocity to v1
            rb2->velocity -= ci.normal * v2; // set rb2 along the normal velocity to v2
        }
        else if (rb1->dynamic) {
            float u1 = glm::dot(-rb1->velocity, ci.normal); // velocity of rb1 along the normal (positive when approaching point of contact)
            rb1->velocity += ci.normal * u1; // set rb1 along the normal velocity to 0
            float v1 = (-u1) * restitution; // final velocity of rb1 along the normal
            rb1->velocity -= ci.normal * v1; // set rb1 along the normal velocity to v1
        }
        else if (rb2->dynamic) {
            float u2 = glm::dot(-rb2->velocity, ci.normal); // velocity of rb2 along the normal (negative when approaching point of contact)
            rb2->velocity += ci.normal * u2; // set rb2 along the normal velocity to 0
            float v2 = (-u2) * restitution; // final velocity of rb2 along the normal
            rb2->velocity -= ci.normal * v2; // set rb2 along the normal velocity to v2
        }
    }
    else {
        if (rb1->dynamic) {
            rb1->temp_force += rb1->get_force(ci.normal * glm::dot(-rb1->force, ci.normal), ci.collision_point); // for every action there is equal and opposite reaction
        }
        if (rb2->dynamic) {
            rb2->temp_force += rb2->get_force(ci.normal * glm::dot(-rb2->force, ci.normal), ci.collision_point); // for every action there is equal and opposite reaction
        }
    }
}


// COLLIDERS

physics::colliders::aabb::aabb(physics::rigidbody* rb, const glm::vec3& size_) : collider(rb), position(rb->position), size(size_) {}

physics::colliders::aabb::aabb(physics::rigidbody* const rb, scripts_system::script* const owner_, const glm::vec3& size_) : collider(rb, owner_), position(rb->position), size(size_) {}

physics::colliders::aabb::aabb(glm::vec3& position_, const glm::vec3& size_) : collider(), position(position_), size(size_) {}

physics::colliders::aabb::aabb(glm::vec3& position_, scripts_system::script* const owner_, const glm::vec3& size_) : collider(owner_), position(position_), size(size_) {}

int physics::colliders::aabb::get_type() { return COLLIDERS_AABB; }

void physics::colliders::aabb::adjust_position(const glm::vec3& collision_point)
{
    // to do
}

physics::colliders::sphere::sphere(physics::rigidbody* rb, const float& radius_) : collider(rb), position(rb->position), radius(radius_) {}

physics::colliders::sphere::sphere(physics::rigidbody* rb, scripts_system::script* const owner_, const float& radius_) : collider(rb, owner_), position(rb->position), radius(radius_) {}

physics::colliders::sphere::sphere(glm::vec3& position_, const float& radius_) : collider(), position(position_), radius(radius_) {}

physics::colliders::sphere::sphere(glm::vec3& position_, scripts_system::script* const owner_, const float& radius_) : collider(owner_), position(position_), radius(radius_) {}

int physics::colliders::sphere::get_type() { return COLLIDERS_SPHERE; }

void physics::colliders::sphere::adjust_position(const glm::vec3& collision_point)
{
    this->position -= glm::normalize(this->position - collision_point) * (glm::length(this->position - collision_point) - this->radius);
}

physics::colliders::plane::plane(physics::rigidbody* rb, const glm::vec3& size_) : collider(rb), position(rb->position), rotation(rb->rotation), size(size_) {}

physics::colliders::plane::plane(physics::rigidbody* rb, scripts_system::script* const owner_, const glm::vec3& size_) : collider(rb, owner_), position(rb->position), rotation(rb->rotation), size(size_) {}

physics::colliders::plane::plane(glm::vec3& position_, glm::quat& rotation_, const glm::vec3& size_) : collider(), position(position_), rotation(rotation_), size(size_) {}

physics::colliders::plane::plane(glm::vec3& position_, scripts_system::script* const owner_, glm::quat& rotation_, const glm::vec3& size_) : collider(owner_), position(position_), rotation(rotation_), size(size_) {}

int physics::colliders::plane::get_type() { return COLLIDERS_PLANE; }

physics::collider::collider(scripts_system::script* const owner_) : owner(owner_), rigidbody(nullptr) { physics::all_colliders.push_back(this); }

physics::collider::collider(physics::rigidbody* const rb, scripts_system::script* const owner_) : owner(owner_), rigidbody(rb) { physics::all_colliders.push_back(this); }

void physics::collider::collision_notify(const physics::collision_info& ci)
{
    this->_collided_this_frame.push_back(ci.other);
    if (std::find(this->_collided_last_frame.begin(), this->_collided_last_frame.end(), ci.other) != this->_collided_last_frame.end()) {
        on_collision_stay.call_events(ci);
    }
    else {
        on_collision_enter.call_events(ci);
    }
}

bool physics::collider::in_collided_last_frame(const physics::collider* const col) const
{
    if (std::find(this->_collided_last_frame.begin(), this->_collided_last_frame.end(), col) != this->_collided_last_frame.end()) {
        return true;
    }
    return false;
}

void physics::collider::swap_collider_buffers()
{
    for (auto& c : _collided_last_frame) {
        if (std::find(this->_collided_this_frame.begin(), this->_collided_this_frame.end(), c) == this->_collided_this_frame.end()) {
            if (this->rigidbody != nullptr) {
            }
            on_collision_exit.call_events();
        }
    }
    this->_collided_last_frame = this->_collided_this_frame;
    this->_collided_this_frame.clear();
}

int physics::collider::get_type() { return COLLIDERS_NONE; }

void physics::collider::adjust_position(const glm::vec3& collision_point) {}

physics::collider::~collider() {
    std::vector<physics::collider*>::iterator id = std::find(physics::all_colliders.begin(), physics::all_colliders.end(), this);
    if (id != physics::all_colliders.end()) physics::all_colliders.erase(id);
}


// COLLISION DETECTION

void physics::run()
{
    // update rigidbodies
    for (rigidbody* rb : rigidbodies) {
        rb->update();
    }

    // check for collisions
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
        ci.normal = glm::normalize(a.position - b.position);
    }
    return ci;
}

physics::collision_info physics::get_collision_info(const colliders::sphere& s, const colliders::plane& p)
{
    collision_info ci;
    glm::vec3 srp = glm::inverse(p.rotation) * (s.position - p.position); // sphere relative position to plane center
    float intersect = s.radius - srp.y;
    ci.collision = (
        srp.x <= p.size.x / 2.0f &&
        srp.x >= -p.size.x / 2.0f &&
        srp.z <= p.size.z / 2.0f &&
        srp.z >= -p.size.z / 2.0f &&
        intersect >= 0.0f &&
        intersect <= p.size.y // thickness limiter
        );
    if (ci.collision) {
        ci.normal = glm::vec3(0.0f, 1.0f, 0.0f) * glm::inverse(p.rotation);
        ci.collision_point = s.position - (ci.normal * (s.radius - (intersect / 2.0f)));
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

