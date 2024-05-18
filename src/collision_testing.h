#pragma once
#include <renderer.h>
#include "scripts_system.h"
#include "physics.h"
#include "entity.h"

namespace physics {
	class collision_test_script : public game::entity {
	private:
		rigidbody rb1;
		rigidbody rb2;
		colliders::sphere sc1;
		colliders::sphere sc2;
		renderer::model m1;
		renderer::model m2;
		void move_sphere1();
		void move_sphere2();
	public:
		collision_test_script();
		void start();
		void update();
	};
}