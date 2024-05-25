#pragma once
#include <functional>
#include "timer.h"

namespace game {
	class player;
}
#include "player_script.h"

namespace game {
	class power_cube {
	public:
		char type = 'a';
		float cooldown = 3.0f;
		int healing = 25;
		time_system::timer t;
		std::function<void()> on_use = [this]() {
			printf("used\n");
			};
		void use();

		power_cube(game::player* owner);
	};
}
