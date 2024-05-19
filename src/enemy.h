#pragma once
#include "entity.h"
#include <timer.h>
#include "physics_object.h"

namespace game {
	class enemy : public game::entity {
	private:
		time_system::timer t;
		
		game::physics_object<physics::colliders::sphere> po;

		void shoot();

	public:
		enemy();
		enemy(const glm::vec3& initial_position, const float& y_rotation);

		void update() override;
	};
}