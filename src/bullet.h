#pragma once
#include "game_script.h"
#include <renderer.h>
#include <rigidbody.h>
#include <physics.h>
#include <physics_object.h>
#include <timer.h>

namespace game {
	class bullet : public scripts_system::script {
	public:
		float damage;
		time_system::timer t;
		game::physics_object<physics::colliders::sphere> po;

		bullet(float size = 1.0f);
		void update() override;

		void hit(physics::collision_info ci);
	};
}