#pragma once
#include <game_script.h>

namespace game {
	class entity : public scripts_system::script {
	public:
		float hp = 10.0f;
		virtual void damage(float damage);
		virtual void die();
	};
}