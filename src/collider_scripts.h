#pragma once
#include "game_script.h"
#include <physics.h>

namespace game {
    namespace colliders {
        class base : public scripts_system::script {
        public:
            base(const glm::vec3& position, const glm::quat& rotation = glm::quat(glm::vec3(1.0f)));
            physics::rigidbody rb;
            // to add model - for rendering for debug
        };
        class sphere : public base {
        public:
            sphere(const glm::vec3& position, const float& radius);
            physics::colliders::sphere col;
        };
        class aabb : public base {
        public:
            aabb(const glm::vec3& position, const glm::vec3& size);
            physics::colliders::aabb col;
        };
        class box : public base {
        public:
            box(const glm::vec3& position, const glm::quat& rotation, const glm::vec3& size);
            physics::colliders::box col;
        };
        class capsule : public base {
        public:
            capsule(const glm::vec3& position, const glm::quat& rotation, const float& radius, const float& spread);
            physics::colliders::capsule col;
        };
        class plane : public base {
        public:
            plane(const glm::vec3& position, const glm::quat& rotation, const glm::vec3& size);
            physics::colliders::plane col;
        };
    }
}
