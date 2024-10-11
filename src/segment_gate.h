#pragma once
#include "gate.h"
namespace game {
    class segment_gate : public gate {
    public:
        segment_gate(
            const glm::vec3& position,
            const glm::quat& rotation_,
            const glm::vec3& size
        );
        std::function<void()> on_pass1_spawn;
        std::function<void()> on_pass1_clear;
        std::function<void()> on_pass2_spawn;
        std::function<void()> on_pass2_clear;
    };
}
