#include "enemy.h"
#include <projectile.h>
#include "scripts_system.h"
#include <textures.h>
#include <mesh.h>


// ENEMIES

game::enemy::preset game::enemies::floater1 = {
	50, // hp
	{ // gun
		false, // auto repeat
		2.3f, // cooldown
		0.0f, // recoil
		0.6f, // scope
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
	},
		"../assets/models/Ghost.obj",
		"../assets/textures/ghost/Ghost_Normal.png",
		"../assets/textures/ghost/Ghost_BaseColor.png",
		"../assets/textures/White_Square.png",
		1.0
};

game::enemy::preset game::enemies::floater2 = {
	60, // hp
	{ // gun
		false, // auto repeat
		2.5f, // cooldown
		0.0f, // recoil
		0.6f, // scope
		[](const glm::vec3& position, const glm::vec3& direction, const int& layer) { // shoot
			game::projectile* proj = scripts_system::instantiate<game::projectile, float>(0.15f);
			proj->on_hit = [](game::entity* ent) {
				ent->damage(12);
			};
			proj->on_miss = []() {};
			proj->po.rb.position = position;
			proj->po.rb.velocity = direction * 45.0f;
			proj->po.rb.mass = 0.1f;
			proj->po.rb.restitution = 0.0f;
			proj->po.col.radius = 0.15f;
			proj->po.col.layer = layer;
		}
	},
	55.0f, // aggro radius
	[](game::enemy* this_enemy, game::player* pl) { // on_aggro
		this_enemy->po.rb.force = glm::vec3(0.0f);
		glm::vec3 dir = game::player::get_closest_player_position(this_enemy->po.rb.position) - this_enemy->po.rb.position;
		if (glm::length(dir) != 0.0f && glm::length(this_enemy->po.rb.velocity) < 10.0f)
			if (glm::length(dir) > 11.0f) this_enemy->po.rb.temp_force += glm::normalize(dir) * 900.0f;
			if (glm::length(dir) < 9.0f) this_enemy->po.rb.temp_force -= glm::normalize(dir) * 900.0f;
	},
		"../assets/models/Ghost.obj",
		"../assets/textures/ghost/Ghost_Normal.png",
		"../assets/textures/ghost/Ghost_BaseColor.png",
		"../assets/textures/White_Square.png",
		1.05
};

game::enemy::preset game::enemies::floater3 = {
	70, // hp
	{ // gun
		false, // auto repeat
		2.1f, // cooldown
		0.0f, // recoil
		0.6f, // scope
		[](const glm::vec3& position, const glm::vec3& direction, const int& layer) { // shoot
			game::projectile* proj = scripts_system::instantiate<game::projectile, float>(0.15f);
			proj->on_hit = [](game::entity* ent) {
				ent->damage(14);
			};
			proj->on_miss = []() {};
			proj->po.rb.position = position;
			proj->po.rb.velocity = direction * 55.0f;
			proj->po.rb.mass = 0.1f;
			proj->po.rb.restitution = 0.0f;
			proj->po.col.radius = 0.15f;
			proj->po.col.layer = layer;
		}
	},
	60.0f, // aggro radius
	[](game::enemy* this_enemy, game::player* pl) { // on_aggro
		this_enemy->po.rb.force = glm::vec3(0.0f);
		glm::vec3 dir = game::player::get_closest_player_position(this_enemy->po.rb.position) - this_enemy->po.rb.position;
		if (glm::length(dir) != 0.0f && glm::length(this_enemy->po.rb.velocity) < 10.0f)
			if (glm::length(dir) > 11.0f) this_enemy->po.rb.temp_force += glm::normalize(dir) * 1100.0f;
			if (glm::length(dir) < 9.0f) this_enemy->po.rb.temp_force -= glm::normalize(dir) * 1100.0f;
	},
		"../assets/models/Ghost.obj",
		"../assets/textures/ghost/Ghost_Normal.png",
		"../assets/textures/ghost/Ghost_BaseColor.png",
		"../assets/textures/White_Square.png",
		1.1
};

game::enemy::preset game::enemies::stationary1 = {
	250, // hp
	{ // gun
		false, // auto repeat
		3.5f, // cooldown
		0.0f, // recoil
		0.6f, // scope
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
	},
		"../assets/models/Tower.obj",
		"../assets/textures/Neutral_Normal.png",
		"../assets/textures/Tower/color.png",
		"../assets/textures/White_Square.png",
		1.1
};

game::enemy::preset game::enemies::stationary2 = {
	270, // hp
	{ // gun
		false, // auto repeat
		4.0f, // cooldown
		0.0f, // recoil
		0.6f, // scope
		[](const glm::vec3& position, const glm::vec3& direction, const int& layer) { // shoot
			game::projectile* proj = scripts_system::instantiate<game::projectile, float>(0.25f);
			proj->on_hit = [](game::entity* ent) {
				ent->damage(30);
			};
			proj->on_miss = []() {};
			proj->po.rb.position = position;
			proj->po.rb.velocity = direction * 28.0f;
			proj->po.rb.mass = 0.2f;
			proj->po.rb.restitution = 0.0f;
			proj->po.col.radius = 0.25f;
			proj->po.col.layer = layer;
		}
	},
	50.0f, // aggro radius
	[](game::enemy* this_enemy, game::player* pl) { // on_aggro
		this_enemy->po.rb.dynamic = false;
	},
		"../assets/models/Tower.obj",
		"../assets/textures/Neutral_Normal.png",
		"../assets/textures/Tower/color.png",
		"../assets/textures/White_Square.png",
		1.0
};

game::enemy::preset game::enemies::stationary3 = {
	300, // hp
	{ // gun
		false, // auto repeat
		3.0f, // cooldown
		0.0f, // recoil
		0.6f, // scope
		[](const glm::vec3& position, const glm::vec3& direction, const int& layer) { // shoot
			game::projectile* proj = scripts_system::instantiate<game::projectile, float>(0.2f);
			proj->on_hit = [](game::entity* ent) {
				ent->damage(20);
			};
			proj->on_miss = []() {};
			proj->po.rb.position = position;
			proj->po.rb.velocity = direction * 35.0f;
			proj->po.rb.mass = 0.2f;
			proj->po.rb.restitution = 0.0f;
			proj->po.col.radius = 0.2f;
			proj->po.col.layer = layer;
		}
	},
	40.0f, // aggro radius
	[](game::enemy* this_enemy, game::player* pl) { // on_aggro
		this_enemy->po.rb.dynamic = false;
	},
		"../assets/models/Tower.obj",
		"../assets/textures/Neutral_Normal.png",
		"../assets/textures/Tower/color.png",
		"../assets/textures/White_Square.png",
		1.1
};

game::enemy::preset game::enemies::sniper1 = {
	40, // hp
	{ // gun
		false, // auto repeat
		5.0f, // cooldown
		0.0f, // recoil
		0.6f, // scope
		[](const glm::vec3& position, const glm::vec3& direction, const int& layer) { // shoot
			game::projectile* proj = scripts_system::instantiate<game::projectile, float>(0.05f);
			proj->on_hit = [](game::entity* ent) {
				ent->damage(50); // High damage for sniper shots
			};
			proj->on_miss = []() {};
			proj->po.rb.position = position;
			proj->po.rb.velocity = direction * 100.0f; // High velocity for sniper shots
			proj->po.rb.mass = 0.1f;
			proj->po.rb.restitution = 0.0f;
			proj->po.col.radius = 0.1f;
			proj->po.col.layer = layer;
		}
	},
	60.0f, // aggro radius
	[](game::enemy* this_enemy, game::player* pl) { // on_aggro
		this_enemy->po.rb.force = glm::vec3(0.0f);
		glm::vec3 dir = game::player::get_closest_player_position(this_enemy->po.rb.position) - this_enemy->po.rb.position;
		if (glm::length(dir) > 30.0f) { // Sniper prefers long-range engagement
			this_enemy->po.rb.temp_force = glm::normalize(dir) * 200.0f; // Adjust force to keep distance
		}
 else {
  this_enemy->po.rb.temp_force = -glm::normalize(dir) * 200.0f; // Move away if too close
}
},
		"../assets/models/snakeguy3.obj",
		"../assets/textures/Neutral_Normal.png",
		"../assets/textures/snakeguy/color.png",
		"../assets/textures/White_Square.png",
	1.0
};

game::enemy::preset game::enemies::sniper2 = {
	35, // hp
	{ // gun
		false, // auto repeat
		6.0f, // cooldown
		0.0f, // recoil
		0.6f, // scope
		[](const glm::vec3& position, const glm::vec3& direction, const int& layer) { // shoot
			game::projectile* proj = scripts_system::instantiate<game::projectile, float>(0.05f);
			proj->on_hit = [](game::entity* ent) {
				ent->damage(55); // High damage for sniper shots
			};
			proj->on_miss = []() {};
			proj->po.rb.position = position;
			proj->po.rb.velocity = direction * 90.0f; // High velocity for sniper shots
			proj->po.rb.mass = 0.1f;
			proj->po.rb.restitution = 0.0f;
			proj->po.col.radius = 0.1f;
			proj->po.col.layer = layer;
		}
	},
	65.0f, // aggro radius
	[](game::enemy* this_enemy, game::player* pl) { // on_aggro
		this_enemy->po.rb.force = glm::vec3(0.0f);
		glm::vec3 dir = game::player::get_closest_player_position(this_enemy->po.rb.position) - this_enemy->po.rb.position;
		if (glm::length(dir) > 35.0f) { // Sniper prefers long-range engagement
			this_enemy->po.rb.temp_force = glm::normalize(dir) * 180.0f; // Adjust force to keep distance
		}
		else {
			this_enemy->po.rb.temp_force = -glm::normalize(dir) * 180.0f; // Move away if too close
		}
	},
		"../assets/models/snakeguy3.obj",
		"../assets/textures/Neutral_Normal.png",
		"../assets/textures/snakeguy/color.png",
		"../assets/textures/White_Square.png",
		1.0
};

game::enemy::preset game::enemies::sniper3 = {
	45, // hp
	{ // gun
		false, // auto repeat
		4.5f, // cooldown
		0.0f, // recoil
		0.6f, // scope
		[](const glm::vec3& position, const glm::vec3& direction, const int& layer) { // shoot
			game::projectile* proj = scripts_system::instantiate<game::projectile, float>(0.05f);
			proj->on_hit = [](game::entity* ent) {
				ent->damage(45); // High damage for sniper shots
			};
			proj->on_miss = []() {};
			proj->po.rb.position = position;
			proj->po.rb.velocity = direction * 110.0f; // High velocity for sniper shots
			proj->po.rb.mass = 0.1f;
			proj->po.rb.restitution = 0.0f;
			proj->po.col.radius = 0.1f;
			proj->po.col.layer = layer;
		}
	},
	55.0f, // aggro radius
	[](game::enemy* this_enemy, game::player* pl) { // on_aggro
		this_enemy->po.rb.force = glm::vec3(0.0f);
		glm::vec3 dir = game::player::get_closest_player_position(this_enemy->po.rb.position) - this_enemy->po.rb.position;
		if (glm::length(dir) > 25.0f) { // Sniper prefers long-range engagement
			this_enemy->po.rb.temp_force = glm::normalize(dir) * 220.0f; // Adjust force to keep distance
		}
		else {
			this_enemy->po.rb.temp_force = -glm::normalize(dir) * 220.0f; // Move away if too close
		}
	},
		"../assets/models/snakeguy3.obj",
		"../assets/textures/Neutral_Normal.png",
		"../assets/textures/snakeguy/color.png",
		"../assets/textures/White_Square.png",
		1.1
};

game::enemy::preset game::enemies::kamikaze1 = {
	30, // hp
	{ // gun
		true, // auto repeat
		1.0f, // cooldown
		0.0f, // recoil
		0.6f, // scope
		[](const glm::vec3& position, const glm::vec3& direction, const int& layer) { // shoot
			game::projectile* proj = scripts_system::instantiate<game::projectile, float>(0.1f);
			proj->on_hit = [](game::entity* ent) {
				ent->damage(20);
			};
			proj->on_miss = []() {};
			proj->po.rb.position = position;
			proj->po.rb.velocity = direction * 50.0f;
			proj->po.rb.mass = 0.05f;
			proj->po.rb.restitution = 0.0f;
			proj->po.col.radius = 0.1f;
			proj->po.col.layer = layer;
		}
	},
	30.0f, // aggro radius
	[](game::enemy* this_enemy, game::player* pl) { // on_aggro
		this_enemy->po.rb.force = glm::vec3(0.0f);
		glm::vec3 dir = game::player::get_closest_player_position(this_enemy->po.rb.position) - this_enemy->po.rb.position;
		if (glm::length(dir) != 0.0f)
			this_enemy->po.rb.temp_force += glm::normalize(dir) * 1300.0f;

		// Kamikaze explodes when close to the player
		if (glm::length(dir) < 2.0f) {
			this_enemy->damage(this_enemy->hp); // Self-destruct
			pl->damage(50); // Damage the player
		}
	},
		"../assets/models/demon.obj",
		"../assets/textures/demon/demon_normal.png",
		"../assets/textures/demon/red.png",
		"../assets/textures/White_Square.png",
		1.0
};

game::enemy::preset game::enemies::kamikaze2 = {
	25, // hp
	{ // gun
		true, // auto repeat
		0.8f, // cooldown
		0.0f, // recoil
		0.6f, // scope
		[](const glm::vec3& position, const glm::vec3& direction, const int& layer) { // shoot
			game::projectile* proj = scripts_system::instantiate<game::projectile, float>(0.1f);
			proj->on_hit = [](game::entity* ent) {
				ent->damage(18);
			};
			proj->on_miss = []() {};
			proj->po.rb.position = position;
			proj->po.rb.velocity = direction * 60.0f;
			proj->po.rb.mass = 0.05f;
			proj->po.rb.restitution = 0.0f;
			proj->po.col.radius = 0.1f;
			proj->po.col.layer = layer;
		}
	},
	35.0f, // aggro radius
	[](game::enemy* this_enemy, game::player* pl) { // on_aggro
		this_enemy->po.rb.force = glm::vec3(0.0f);
		glm::vec3 dir = game::player::get_closest_player_position(this_enemy->po.rb.position) - this_enemy->po.rb.position;
		if (glm::length(dir) != 0.0f)
			this_enemy->po.rb.temp_force += glm::normalize(dir) * 1400.0f;

		// Kamikaze explodes when close to the player
		if (glm::length(dir) < 2.5f) {
			this_enemy->damage(this_enemy->hp); // Self-destruct
			pl->damage(55); // Damage the player
		}
	},
		"../assets/models/demon.obj",
		"../assets/textures/demon/demon_normal.png",
		"../assets/textures/demon/red.png",
		"../assets/textures/White_Square.png",
		1.0
};

game::enemy::preset game::enemies::kamikaze3 = {
	35, // hp
	{ // gun
		true, // auto repeat
		1.2f, // cooldown
		0.0f, // recoil
		0.6f, // scope
		[](const glm::vec3& position, const glm::vec3& direction, const int& layer) { // shoot
			game::projectile* proj = scripts_system::instantiate<game::projectile, float>(0.1f);
			proj->on_hit = [](game::entity* ent) {
				ent->damage(22);
			};
			proj->on_miss = []() {};
			proj->po.rb.position = position;
			proj->po.rb.velocity = direction * 45.0f;
			proj->po.rb.mass = 0.05f;
			proj->po.rb.restitution = 0.0f;
			proj->po.col.radius = 0.1f;
			proj->po.col.layer = layer;
		}
	},
	25.0f, // aggro radius
	[](game::enemy* this_enemy, game::player* pl) { // on_aggro
		this_enemy->po.rb.force = glm::vec3(0.0f);
		glm::vec3 dir = game::player::get_closest_player_position(this_enemy->po.rb.position) - this_enemy->po.rb.position;
		if (glm::length(dir) != 0.0f)
			this_enemy->po.rb.temp_force += glm::normalize(dir) * 1200.0f;

		// Kamikaze explodes when close to the player
		if (glm::length(dir) < 1.5f) {
			this_enemy->damage(this_enemy->hp); // Self-destruct
			pl->damage(60); // Damage the player
		}
	},
		"../assets/models/demon.obj",
		"../assets/textures/demon/demon_normal.png",
		"../assets/textures/demon/red.png",
		"../assets/textures/White_Square.png",
		1.0
};

game::enemy::preset game::enemies::tank1 = {
	200, // hp
	{ // gun
		false, // auto repeat
		3.0f, // cooldown
		0.0f, // recoil
		0.6f, // scope
		[](const glm::vec3& position, const glm::vec3& direction, const int& layer) { // shoot
			game::projectile* proj = scripts_system::instantiate<game::projectile, float>(0.5f);
			proj->on_hit = [](game::entity* ent) {
				ent->damage(50); // High damage for area attack
			};
			proj->on_miss = []() {};
			proj->po.rb.position = position;
			proj->po.rb.velocity = direction * 20.0f; // Slow velocity
			proj->po.rb.mass = 1.0f;
			proj->po.rb.restitution = 0.0f;
			proj->po.col.radius = 0.5f; // Larger radius for area effect
			proj->po.col.layer = layer;
		}
	},
	25.0f, // aggro radius
	[](game::enemy* this_enemy, game::player* pl) { // on_aggro
		this_enemy->po.rb.force = glm::vec3(0.0f);
		glm::vec3 dir = game::player::get_closest_player_position(this_enemy->po.rb.position) - this_enemy->po.rb.position;
		if (glm::length(dir) != 0.0f && glm::length(this_enemy->po.rb.velocity) < 5.0f)
			this_enemy->po.rb.temp_force += glm::normalize(dir) * 100.0f; // Slow movement towards the player
	},
		"../assets/models/monster.obj",
		"../assets/textures/Neutral_Normal.png",
		"../assets/textures/monster/color.png",
		"../assets/textures/White_Square.png",
		1.0
};

game::enemy::preset game::enemies::tank2 = {
	250, // hp
	{ // gun
		false, // auto repeat
		3.5f, // cooldown
		0.0f, // recoil
		0.6f, // scope
		[](const glm::vec3& position, const glm::vec3& direction, const int& layer) { // shoot
			game::projectile* proj = scripts_system::instantiate<game::projectile, float>(0.5f);
			proj->on_hit = [](game::entity* ent) {
				ent->damage(55); // High damage for area attack
			};
			proj->on_miss = []() {};
			proj->po.rb.position = position;
			proj->po.rb.velocity = direction * 18.0f; // Slow velocity
			proj->po.rb.mass = 1.0f;
			proj->po.rb.restitution = 0.0f;
			proj->po.col.radius = 0.5f; // Larger radius for area effect
			proj->po.col.layer = layer;
		}
	},
	30.0f, // aggro radius
	[](game::enemy* this_enemy, game::player* pl) { // on_aggro
		this_enemy->po.rb.force = glm::vec3(0.0f);
		glm::vec3 dir = game::player::get_closest_player_position(this_enemy->po.rb.position) - this_enemy->po.rb.position;
		if (glm::length(dir) != 0.0f && glm::length(this_enemy->po.rb.velocity) < 5.0f)
			this_enemy->po.rb.temp_force += glm::normalize(dir) * 80.0f; // Even slower movement towards the player
	},
		"../assets/models/monster.obj",
		"../assets/textures/Neutral_Normal.png",
		"../assets/textures/monster/color.png",
		"../assets/textures/White_Square.png",
		1.1
};

game::enemy::preset game::enemies::tank3 = {
	180, // hp
	{ // gun
		false, // auto repeat
		2.5f, // cooldown
		0.0f, // recoil
		0.6f, // scope
		[](const glm::vec3& position, const glm::vec3& direction, const int& layer) { // shoot
			game::projectile* proj = scripts_system::instantiate<game::projectile, float>(0.5f);
			proj->on_hit = [](game::entity* ent) {
				ent->damage(45); // High damage for area attack
			};
			proj->on_miss = []() {};
			proj->po.rb.position = position;
			proj->po.rb.velocity = direction * 25.0f; // Slow velocity
			proj->po.rb.mass = 1.0f;
			proj->po.rb.restitution = 0.0f;
			proj->po.col.radius = 0.5f; // Larger radius for area effect
			proj->po.col.layer = layer;
		}
	},
	20.0f, // aggro radius
	[](game::enemy* this_enemy, game::player* pl) { // on_aggro
		this_enemy->po.rb.force = glm::vec3(0.0f);
		glm::vec3 dir = game::player::get_closest_player_position(this_enemy->po.rb.position) - this_enemy->po.rb.position;
		if (glm::length(dir) != 0.0f && glm::length(this_enemy->po.rb.velocity) < 5.0f)
			this_enemy->po.rb.temp_force += glm::normalize(dir) * 120.0f; // Slow movement towards the player
	},
		"../assets/models/monster.obj",
		"../assets/textures/Neutral_Normal.png",
		"../assets/textures/monster/color.png",
		"../assets/textures/White_Square.png",
		1.0
};





void game::enemy::shoot()
{
	glm::vec3 dir = game::player::get_closest_player_position(this->po.rb.position) - this->po.rb.position;
	if (glm::length(dir) != 0.0f)
		this->gun.shoot(this->po.rb.position, glm::normalize(dir), COLLISION_LAYERS_ENEMY_PROJECTILES);

	shoot_cooldown.start(this->gun.cooldown);
}

game::enemy::enemy(const game::enemy::preset& preset, const glm::vec3& initial_position, const glm::quat& initial_rotation) :
	entity(), po(glm::vec3(preset.scale)), gun(preset.gun), aggro(this->po.rb.position, this, preset.aggro_radius),
	on_aggro(preset.on_aggro), shoot_cooldown(std::bind(&game::enemy::shoot, this)) 
{
	// entity
	this->hp = preset.hp;
	this->max_hp = preset.hp;

	//model
	this->po.model.mesh = renderer::mesh::get_mesh(preset.mesh);
	this->po.model.diffuse = renderer::get_texture(preset.diffuse);
	this->po.model.normal = renderer::get_texture(preset.normal);
	this->po.model.height = renderer::get_texture(preset.height);
	this->po.model.model_matrix = glm::scale(this->po.model.model_matrix, glm::vec3(preset.scale));
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
