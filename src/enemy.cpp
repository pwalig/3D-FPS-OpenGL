#include "enemy.h"
#include <projectile.h>
#include "scripts_system.h"
#include <gameplay_manager.h>

void game::enemy::shoot()
{
	game::projectile* proj = scripts_system::instantiate<game::projectile, float>(0.15f, this);
	proj->po.rb.position = glm::vec3(0.0f, 5.0f, 0.0f);
	proj->po.rb.velocity = glm::normalize(*game::gameplay_manager::player_position - glm::vec3(0.0f, 5.0f, 0.0f)) * 50.0f;
	proj->po.rb.mass = 0.0000000001f;
	proj->po.rb.force = physics::gravity * proj->po.rb.mass / 10.0f;
	proj->po.rb.restitution = 0.0f;
	proj->po.col.radius = 0.15f;

	t.start(3.0f);
}

game::enemy::enemy()
{
	t.events.subscribe(std::bind(&game::enemy::shoot, this));
	t.start(3.0f);
}

game::enemy::enemy(const float& hp_)
{
	this->hp = hp_;
	t.events.subscribe(std::bind(&game::enemy::shoot, this));
	t.start(3.0f);
}
