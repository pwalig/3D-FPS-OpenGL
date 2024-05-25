#pragma once
#include "entity.h"
#include <timer.h>
#include "physics_object.h"
#include <projectile_damage_gun.h>

namespace game {
	class simple_enemy : public game::entity {
	private:
		time_system::function_timer ft;
		game::physics_object<physics::colliders::sphere> po;
		game::projectile_damage_gun gun;

		void shoot();

	public:
		simple_enemy();
		simple_enemy(const glm::vec3& initial_position, const float& y_rotation);

		void update() override;
	};
}