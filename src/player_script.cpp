#include "player_script.h"
#include <renderer.h>
#include <physics.h>
#include <input_system.h>
#include <axis.h>
#include <constants.h>
#include <time_system.h>
#include <scripts_system.h>

namespace game {
	namespace player {
		physics::rigidbody rb;
		physics::colliders::sphere col(&rb, 1.5f);

		// temporary - make floor for player
		physics::rigidbody rb_floor;
		physics::colliders::plane col_floor(&rb_floor, glm::vec2(50.0f, 50.0f));

		float max_speed = 7.0f; // [m/s]
		float responsiveness = 1.0f; // [m/s^2]
		float ground_responsiveness = 100.0f; // [m/s^2]
		float air_responsiveness = 20.0f; // [m/s^2]

		float jump_force = 5.5f; // actually velocity [m/s]
		bool ready_to_jump = false;

		float rot_speed = PI * 0.5f;
		float max_rot = PI * 0.49f;
		glm::vec2 rot = glm::vec2(0.0f);

		input_system::double_axis move_in = input_system::double_axis(GLFW_KEY_A, GLFW_KEY_D, GLFW_KEY_W, GLFW_KEY_S);
		
		void start();
		void update();
		void jump();
		void land(physics::collision_info ci);
	}
}

void game::player::init() {
	scripts_system::events[SCRIPTS_START].subscribe(start);
	scripts_system::events[SCRIPTS_UPDATE].subscribe(update);
}

void game::player::start()
{
	// set up rigidbody and collider
	rb.mass = 80.0f;
	rb.force = physics::gravity * rb.mass;
	rb.moment_of_inertia = 999999.9f; // make player almost unrotatable
	rb.restitution = 0.0f; // make player not bouncy
	rb.position = glm::vec3(0.0f, 2.0f, -15.0f);
	physics::rigidbodies.push_back(&rb);
	physics::all_colliders.push_back(&col);

	// subscribe for collision handling and input
	col.on_collision_enter.subscribe(land);
	input_system::subscribe(jump, GLFW_KEY_SPACE, GLFW_PRESS);

	// temporary - make floor for player
	rb_floor.restitution = 0.0f;
	physics::all_colliders.push_back(&col_floor);
	renderer::model m1;
	m1.model_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(50.0f, 0.01f, 50.0f));
	renderer::all_models.push_back(m1);
}

void game::player::update()
{
	glm::vec2 mouse_move(input_system::mouse_delta[0], input_system::mouse_delta[1]);
	
	rot += glm::vec2(mouse_move.y * rot_speed, mouse_move.x * rot_speed) * (float)time_system::delta_time;
	if (rot.x > max_rot) rot.x = max_rot;
	if (rot.x < -max_rot) rot.x = -max_rot;


	rb.rotation = glm::rotate(glm::quat(glm::vec3(0, 0, 0)), rot.y, glm::vec3(0.0f, 1.0f, 0.0f)); // rotate around y axis only to preserve movement on xz plane

	glm::vec3 move_dir = rb.rotation * glm::vec3(move_in.normalized().x, 0.0f, move_in.normalized().y);
	float y_vel = rb.velocity.y;
	rb.velocity.y = 0.0f;
	if (glm::length(move_in.normalized()) != 0.0f) {
		rb.velocity += responsiveness * (float)time_system::delta_time * move_dir;
	}
	else {
		if (responsiveness * (float)time_system::delta_time <= glm::length(rb.velocity)) rb.velocity -= responsiveness * (float)time_system::delta_time * glm::normalize(rb.velocity);
		else rb.velocity = glm::vec3(0.0f);
	}
	if (glm::length(rb.velocity) > max_speed) rb.velocity = glm::normalize(rb.velocity) * max_speed;
	rb.velocity.y = y_vel;

	glm::vec3 dir = glm::toMat3(glm::rotate(rb.rotation, rot.x, glm::vec3(1.0f, 0.0f, 0.0f))) * glm::vec3(0, 0, 1); // rotate on x axis (up down) and calculate look direction

	renderer::V = glm::lookAt(rb.position, rb.position + dir, glm::vec3(0.0f, 1.0f, 0.0f));
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
