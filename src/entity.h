#pragma once
#include <game_script.h>
#include <glm/glm.hpp>

namespace game {
	class entity : public scripts_system::script {
	public:
		int hp = 100;
		int max_hp = 100;
		virtual void damage(int damage, glm::vec3 damage_source_position);
		virtual void heal(int healing);
		virtual void die();
	};
}