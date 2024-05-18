#pragma once
#include "entity.h"
#include <timer.h>
namespace game {
	class enemy : public game::entity {
	private:
		time_system::timer t;
		void shoot();

	public:
		enemy();
		enemy(const float& hp_);
	};
}