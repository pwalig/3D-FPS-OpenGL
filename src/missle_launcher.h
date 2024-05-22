#pragma once
#include "weapon.h"
namespace game {
	class missle_launcher : public game::weapon {
	public:
		void shoot(const glm::vec3& position, const glm::vec3& direction, const int& layer) override;
	};
}
