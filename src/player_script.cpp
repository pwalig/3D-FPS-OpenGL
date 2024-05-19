#include "player_script.h"
#include <renderer.h>
#include <time_system.h>
#include <projectile.h>
#include "gameplay_manager.h"


game::player::player(const glm::vec3& initial_position, const float& y_rotation) : rb(), col(&rb, this, 1.5f), dir(glm::vec3(0.0f, 0.0f, 1.0f)) {
	// set up rigidbody
	rb.mass = 80.0f;
	rb.force = physics::gravity * rb.mass;
	rb.moment_of_inertia = 999999.9f; // make player almost unrotatable
	rb.restitution = 0.0f; // make player not bouncy
	rb.position = initial_position;
	rb.rotation = glm::rotate(glm::quat(glm::vec3(0.0f)), y_rotation, glm::vec3(glm::vec3(0.0f, 1.0f, 0.0f)));

	// subscribe for collision event
	col.on_collision_enter.subscribe(std::bind(&game::player::land, this, std::placeholders::_1));
}

void game::player::start()
{
	game::gameplay_manager::player_position = &(this->rb.position);
}

void game::player::update()
{
	glm::vec2 mouse_move(input_system::mouse_delta[0], input_system::mouse_delta[1]);
	
	rot += glm::vec2(mouse_move.y * rot_speed, mouse_move.x * rot_speed) * (float)time_system::delta_time;
	if (rot.x > max_rot) rot.x = max_rot;
	if (rot.x < -max_rot) rot.x = -max_rot;


	rb.rotation = glm::rotate(glm::quat(glm::vec3(0.0f)), rot.y, glm::vec3(0.0f, 1.0f, 0.0f)); // rotate around y axis only to preserve movement on xz plane

	glm::vec3 move_dir = rb.rotation * glm::vec3(move_in.normalized().x, 0.0f, move_in.normalized().y);
	float y_vel = rb.velocity.y;
	rb.velocity.y = 0.0f;
	if (glm::length(move_in.normalized()) != 0.0f) {
		rb.velocity += responsiveness * (float)time_system::delta_time * move_dir;
	}
	else if (glm::length(rb.velocity) > 0.0f) {
		if (responsiveness * (float)time_system::delta_time <= glm::length(rb.velocity)) rb.velocity -= responsiveness * (float)time_system::delta_time * glm::normalize(rb.velocity);
		else rb.velocity = glm::vec3(0.0f);
	}
	if (glm::length(rb.velocity) > max_speed) rb.velocity = glm::normalize(rb.velocity) * max_speed;
	rb.velocity.y = y_vel;

	dir = glm::rotate(rb.rotation, rot.x, glm::vec3(1.0f, 0.0f, 0.0f)) * glm::vec3(0, 0, 1); // rotate on x axis (up down) and calculate look direction

	renderer::V = glm::lookAt(rb.position, rb.position + dir, glm::vec3(0.0f, 1.0f, 0.0f));
}

void game::player::die()
{
	printf("you died\n");
	this->hp = 0.0f;
}

void game::player::jump()
{
	if (ready_to_jump) {
		ready_to_jump = false;
		responsiveness = air_responsiveness;
		rb.velocity.y += jump_force;
	}
}

void game::player::land(physics::collision_info ci) {
	if (!ready_to_jump) {
		ready_to_jump = true;
		responsiveness = ground_responsiveness;
	}
	if (input_system::key_held[GLFW_KEY_SPACE]) jump();
 }

void game::player::shoot()
{
	game::projectile* proj = scripts_system::instantiate<game::projectile, float>(0.15f, this);
	proj->po.rb.position = this->rb.position + (this->dir * 3.0f);
	proj->po.rb.velocity = this->dir * 50.0f;
	proj->po.rb.mass = 0.0000000001f;
	proj->po.rb.force = physics::gravity * proj->po.rb.mass / 10.0f;
	proj->po.rb.restitution = 0.0f;
	proj->po.col.radius = 0.15f;
}

void game::player::shoot_ray()
{
	physics::ray_intersection_info ri = physics::ray_cast(physics::ray(this->rb.position, this->dir, COLLISION_LAYERS_PLAYER_PROJECTILES));
	if (ri.intersect == RAY_INTERSECT_NONE) { printf("no ray hit\n"); return; }
	printf("ri: %s, %d, %f\n", ri.col->owner->name.c_str(), ri.intersect, ri.distance);
	if (game::entity* ent = dynamic_cast<game::entity*>(ri.col->owner)) {
		// we collided with entity
		ent->damage(1.0f);
		printf("new hp = %f\n", ent->hp);

		if (ent->hp <= 0.0f) {
			ent->die();
		}
	}
}
