#pragma once
#include "weapon.h"
namespace game {
	class projectile_damage_gun : public game::weapon {
	public:
		int max_damage = 11;
		int min_damage = 9;
		projectile_damage_gun(const int& max_damage_, const int& min_damage_);
	};
}
