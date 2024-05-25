#include "simple_enemy.h"
#include <projectile.h>
#include "scripts_system.h"
#include <gameplay_manager.h>

void game::simple_enemy::shoot()
{
	this->gun.shoot(this->po.rb.position, glm::normalize(*game::gameplay_manager::player_position - po.rb.position), COLLISION_LAYERS_ENEMY_PROJECTILES);

	ft.start(this->gun.cooldown);
}

game::simple_enemy::simple_enemy() : po(), gun(10,10), ft(this->gun.cooldown, std::bind(&game::simple_enemy::shoot, this))
{
	po.name = this->name + "_po";
	po.col.layer = COLLISION_LAYERS_ENEMIES;
	po.col.owner = this;
	po.rb.mass = 50.0f;
	po.rb.force = physics::gravity * po.rb.mass;
	gun.on_hit = [this](game::entity* ent) {
		ent->damage(10);
		};
	gun.cooldown = 2.5f;
}

game::simple_enemy::simple_enemy(const glm::vec3& initial_position, const float& y_rotation) : simple_enemy()
{
	po.rb.position = initial_position;
	po.rb.rotation = glm::rotate(po.rb.rotation, y_rotation, glm::vec3(0.0f, 1.0f, 0.0f));
}

void game::simple_enemy::update()
{
	po.rb.temp_force += glm::normalize(*gameplay_manager::player_position - po.rb.position) * po.rb.mass;
}
