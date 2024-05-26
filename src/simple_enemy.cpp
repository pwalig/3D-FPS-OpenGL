#include "simple_enemy.h"
#include <projectile.h>
#include "scripts_system.h"
#include <gameplay_manager.h>
#include <renderer.h>

void game::simple_enemy::shoot()
{
	game::projectile* proj = scripts_system::instantiate<game::projectile, float>(0.15f, this);
	proj->po.rb.position = po.rb.position;
	proj->po.rb.velocity = glm::normalize(*game::gameplay_manager::player_position - po.rb.position) * 50.0f;
	proj->po.rb.mass = 0.1f;
	proj->po.rb.force = physics::gravity * proj->po.rb.mass * 0.0001f;
	proj->po.rb.restitution = 0.0f;
	proj->po.col.radius = 0.15f;
	proj->po.col.layer = COLLISION_LAYERS_ENEMY_PROJECTILES;

	t.start(2.5f);
}

game::simple_enemy::simple_enemy() : po()
{
	po.name = this->name + "_po";
	po.col.layer = COLLISION_LAYERS_ENEMIES;
	po.col.owner = this;
	po.rb.mass = 50.0f;
	po.rb.force = physics::gravity * po.rb.mass;
	t.events.subscribe(std::bind(&game::simple_enemy::shoot, this));
	t.start(2.5f);
	this->po.model.mesh = renderer::get_mesh("..\\assets\\objects\\jet_axe.obj");
	this->po.model.diffuse = renderer::get_texture("..\\assets\\objects\\Axe_Albedo.png");
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
