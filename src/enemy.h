#pragma once
#include <physics_object.h>
#include <timer.h>
#include <weapon.h>
#include <player_script.h>
#include <string>

namespace game {
    class enemy : public game::entity {
	public:
		time_system::function_timer shoot_cooldown;
		game::physics_object<physics::colliders::sphere> po;
		game::weapon gun;
		physics::colliders::sphere aggro;
		std::function<void(game::enemy* this_enemy, game::player* pl)> on_aggro;

		void shoot();

		struct preset {
			int hp;
			game::weapon gun;
			// to do - struct model::preset
			float aggro_radius;
			std::function<void(game::enemy* this_enemy, game::player* pl)> on_aggro;
			std::string mesh;
			std::string normal;
			std::string diffuse;
			std::string height;
			float scale;
		};

		enemy(const game::enemy::preset& preset, const glm::vec3& initial_position = glm::vec3(0.0f), const glm::quat& initial_rotation = glm::quat(glm::vec3(0.0f)));

		void update() override;
	};

	namespace enemies {
		extern game::enemy::preset floater1;
		extern game::enemy::preset stationary1;
		extern game::enemy::preset kamikaze1;
		extern game::enemy::preset sniper1;
		extern game::enemy::preset tank1;
		extern game::enemy::preset floater2;
		extern game::enemy::preset stationary2;
		extern game::enemy::preset kamikaze2;
		extern game::enemy::preset sniper2;
		extern game::enemy::preset tank2;
		extern game::enemy::preset floater3;
		extern game::enemy::preset stationary3;
		extern game::enemy::preset kamikaze3;
		extern game::enemy::preset sniper3;
		extern game::enemy::preset tank3;
	}
}

