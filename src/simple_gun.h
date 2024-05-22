#pragma once
#include "weapon.h"
namespace game {
	class simple_gun : public game::weapon {
	public:
		int damage = 10;
		void shoot(const glm::vec3& position, const glm::vec3& direction, const int& layer) override;
	};
}

