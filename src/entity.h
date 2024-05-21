#pragma once
#include <game_script.h>

namespace game {
	class entity : public scripts_system::script {
	public:
		int hp = 100;
		virtual void damage(int damage);
		virtual void die();
	};
}