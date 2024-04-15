#pragma once
#include <physics.h>

namespace game {
	namespace player {
		extern physics::rigidbody rb;
		void init();
		void start();
		void update();
		void jump();
	}
}