#include "enemy.h"
#include <projectile.h>
#include "scripts_system.h"
#include <gameplay_manager.h>

void game::enemy::shoot()
{
	game::projectile* proj = scripts_system::instantiate<game::projectile, float>(0.15f, this);
	proj->po.rb.position = po.rb.position;
	proj->po.rb.velocity = glm::normalize(*game::gameplay_manager::player_position - po.rb.position) * 50.0f;
	proj->po.rb.mass = 0.0000000001f;
	proj->po.rb.force = physics::gravity * proj->po.rb.mass / 10.0f;
	proj->po.rb.restitution = 0.0f;
	proj->po.col.radius = 0.15f;
	proj->po.col.layer = COLLISION_LAYERS_ENEMY_PROJECTILES;

	t.start(2.5f);
}

game::enemy::enemy() : po()
{
	po.col.layer = COLLISION_LAYERS_ENEMIES;
	po.col.owner = this;
	po.rb.mass = 50.0f;
	po.rb.force = physics::gravity * po.rb.mass;
	t.events.subscribe(std::bind(&game::enemy::shoot, this));
	t.start(2.5f);
}

game::enemy::enemy(const glm::vec3& initial_position, const float& y_rotation) : enemy()
{
	po.rb.position = initial_position;
	po.rb.rotation = glm::rotate(po.rb.rotation, y_rotation, glm::vec3(0.0f, 1.0f, 0.0f));
}

void game::enemy::update()
{
	po.rb.temp_force += glm::normalize(*gameplay_manager::player_position - po.rb.position) * po.rb.mass;
	po.update();
}
