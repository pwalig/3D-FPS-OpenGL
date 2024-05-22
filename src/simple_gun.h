#pragma once
#include "weapon.h"
namespace game {
	class simple_gun : public game::weapon {
	public:
		int damage = 10;
		simple_gun();
	};
}

