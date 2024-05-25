#pragma once
#include "game_script.h"
#include <renderer.h>
#include <rigidbody.h>
#include <physics.h>
#include <physics_object.h>
#include <timer.h>
#include <entity.h>

namespace game {
	class projectile : public scripts_system::script {
	public:
		time_system::function_timer ft;
		game::physics_object<physics::colliders::sphere> po;

		projectile(float size = 1.0f);
		void update() override;

		std::function<void(game::entity*)> on_hit;
		std::function<void()> on_miss = []() {};
		void hit(physics::collision_info ci);
	};
}