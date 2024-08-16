#pragma once
#include "entity.h"
#include <physics_object.h>
#include <key_bind.h>
#include "glfw/glfw3.h"

namespace game {
	class dummy : public entity {
	public:
		float time = 0.0f;
		int damage_sum = 0;
		game::physics_object<physics::colliders::aabb> po;
		void damage(int damage, glm::vec3 damage_source_position) override;
		input_system::key_bind reset_key_bind = input_system::key_bind([this]() {this->damage_sum = 0; this->time = 0.0f; }, GLFW_KEY_P, GLFW_PRESS); // reset dummy damage sum and dps counting

		dummy();
		void update() override;
	};
}

