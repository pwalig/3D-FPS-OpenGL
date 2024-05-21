#pragma once
#include "projectile.h"

namespace game {
	class homing_projectile : public game::projectile {
	public:
		static void homing_none(homing_projectile* hproj, const glm::vec3& target);
		static void homing_force_normalized(homing_projectile* hproj, const glm::vec3& target);
		static void homing_force_spring(homing_projectile* hproj, const glm::vec3& target);
		static void homing_force_gravity(homing_projectile* hproj, const glm::vec3& target);
		static void homing_rotation_normalized(homing_projectile* hproj, const glm::vec3& target);
		static void homing_rotation_spring(homing_projectile* hproj, const glm::vec3& target);
		static void homing_velocity_normalized(homing_projectile* hproj, const glm::vec3& target);
		static void homing_velocity_spring(homing_projectile* hproj, const glm::vec3& target);

		glm::vec3& target;
		float homing_force = 10.0f;
		void (*homing_function)(homing_projectile*, const glm::vec3&) = &homing_projectile::homing_force_normalized;

		homing_projectile(glm::vec3& target_, const float& size);
		void update() override;
	};
}
