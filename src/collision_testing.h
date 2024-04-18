#pragma once
#include "scripts_system.h"
#include "collision_detection.h"

namespace physics {
	class collision_test_script : public scripts_system::script {
	private:
		colliders::sphere sc1;
		colliders::sphere sc2;
		glm::vec3 pos1;
		glm::vec3 pos2;
	public:
		collision_test_script();
		void move_sphere1();
		void move_sphere2();
		void start();
	};
}