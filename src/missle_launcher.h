#pragma once
#include "weapon.h"
namespace game {
	class missle_launcher : public game::weapon {
	public:
		float damage = 10;
		missle_launcher();
	};
}
