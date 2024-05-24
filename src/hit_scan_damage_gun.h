#pragma once
#include "weapon.h"
namespace game {
	class hit_scan_damage_gun : public game::weapon {
	public:
		int max_damage = 11;
		int min_damage = 9;
		hit_scan_damage_gun(const int& max_damage_, const int& min_damage_);
	};
}

