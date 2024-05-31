#include "enemy.h"
#include <projectile.h>
#include "scripts_system.h"


// ENEMIES

game::enemy::preset game::enemies::floater1 = {
	50, // hp
	{ // gun
		false, // auto repeat
		2.3f, // cooldown
		0.0f, // recoil
		[](const glm::vec3& position, const glm::vec3& direction, const int& layer) { // shoot
			game::projectile* proj = scripts_system::instantiate<game::projectile, float>(0.15f);
			proj->on_hit = [](game::entity* ent) {
				ent->damage(10);
			};
			proj->on_miss = []() {};
			proj->po.rb.position = position;
			proj->po.rb.velocity = direction * 50.0f;
			proj->po.rb.mass = 0.1f;
			proj->po.rb.restitution = 0.0f;
			proj->po.col.radius = 0.15f;
			proj->po.col.layer = layer;
		}
	},
	50.0f, // aggro radius
	[](game::enemy* this_enemy, game::player* pl) { // on_aggro
		this_enemy->po.rb.force = glm::vec3(0.0f);
		glm::vec3 dir = game::player::get_closest_player_position(this_enemy->po.rb.position) - this_enemy->po.rb.position;
		if (glm::length(dir) != 0.0f && glm::length(this_enemy->po.rb.velocity) < 10.0f)
			if (glm::length(dir) > 11.0f) this_enemy->po.rb.temp_force += glm::normalize(dir) * 1000.0f;
			if (glm::length(dir) < 9.0f) this_enemy->po.rb.temp_force -= glm::normalize(dir) * 1000.0f;
	}
};

game::enemy::preset game::enemies::stationary1 = {
	150, // hp
	{ // gun
		false, // auto repeat
		3.5f, // cooldown
		0.0f, // recoil
		[](const glm::vec3& position, const glm::vec3& direction, const int& layer) { // shoot
			game::projectile* proj = scripts_system::instantiate<game::projectile, float>(0.2f);
			proj->on_hit = [](game::entity* ent) {
				ent->damage(25);
			};
			proj->on_miss = []() {};
			proj->po.rb.position = position;
			proj->po.rb.velocity = direction * 30.0f;
			proj->po.rb.mass = 0.2f;
			proj->po.rb.restitution = 0.0f;
			proj->po.col.radius = 0.2f;
			proj->po.col.layer = layer;
		}
	},
	45.0f, // aggro radius
	[](game::enemy* this_enemy, game::player* pl) { // on_aggro
		this_enemy->po.rb.dynamic = false;
	}
};

void game::enemy::shoot()
{
	glm::vec3 dir = game::player::get_closest_player_position(this->po.rb.position) - this->po.rb.position;
	if (glm::length(dir) != 0.0f)
		this->gun.shoot(this->po.rb.position, glm::normalize(dir), COLLISION_LAYERS_ENEMY_PROJECTILES);

	shoot_cooldown.start(this->gun.cooldown);
}

game::enemy::enemy(const game::enemy::preset& preset, const glm::vec3& initial_position, const glm::quat& initial_rotation) :
	entity(), po(), gun(preset.gun), aggro(this->po.rb.position, this, preset.aggro_radius),
	on_aggro(preset.on_aggro), shoot_cooldown(std::bind(&game::enemy::shoot, this))
{
	// entity
	this->hp = preset.hp;
	this->max_hp = preset.hp;

	// prepare physics_object
	po.rb.position = initial_position;
	po.rb.rotation = initial_rotation;
	po.name = this->name + "_po";
	po.col.layer = COLLISION_LAYERS_ENEMIES;
	po.col.owner = this;
	po.rb.mass = 50.0f;
	po.rb.force = physics::gravity * po.rb.mass;
	po.rb.movement_drag = 100.0f;

	// prepare aggro collider
	aggro.layer = COLLISION_LAYERS_AGGRO;
	aggro.on_collision_enter.subscribe([this](physics::collision_info ci) {
		if (this->shoot_cooldown.time == 0.0f)
			this->shoot_cooldown.start(this->gun.cooldown);
		});
	aggro.on_collision_stay.subscribe([this](physics::collision_info ci) {
		this->on_aggro(this, game::player::get_closest_player(this->po.rb.position));
		});
	aggro.on_collision_exit.subscribe([this]() {
		this->shoot_cooldown.stop();
		});
}

void game::enemy::update()
{
}
