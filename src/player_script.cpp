#include "player_script.h"
#include <renderer.h>
#include <physics.h>
#include <input_system.h>
#include <axis.h>
#include <constants.h>

namespace game {
	namespace player {
		physics::rigidbody rb;

		float max_speed = 7.0f; // [m/s]
		float responsiveness = 20.0f; // [m/s^2]

		float jump_force = 7.0f;
		bool ready_to_jump = true;

		float rot_speed = PI * 0.05f;
		float max_rot = PI * 1.95f;
		glm::vec2 rot = glm::vec2(0.0f);

		input_system::double_axis move_in = input_system::double_axis(GLFW_KEY_A, GLFW_KEY_D, GLFW_KEY_W, GLFW_KEY_S);
		input_system::double_axis rot_in = input_system::double_axis(GLFW_KEY_DOWN, GLFW_KEY_UP, GLFW_KEY_LEFT, GLFW_KEY_RIGHT);
	}
}


void game::player::start()
{
	rb.mass = 80.0f;
	responsiveness *= rb.mass;
	rb.moment_of_inertia = 99999.9f;
	rb.position = glm::vec3(0.0f, 1.5f, -15.0f);
	input_system::subscribe(jump, GLFW_KEY_SPACE, GLFW_PRESS);
}

void game::player::movement()
{
	rot += rot_in.normalized() * rot_speed;
	if (rot.x > max_rot) rot.x = max_rot;
	if (rot.x < -max_rot) rot.x = -max_rot;

	rb.rotation = glm::rotate(glm::quat(glm::vec3(0, 0, 0)), rot_speed * rot.y, glm::vec3(0.0f, 1.0f, 0.0f));

	if (glm::length(rb.velocity) < max_speed && glm::length(move_in.normalized()) != 0.0f) {
		rb.force = rb.rotation * glm::vec3(move_in.normalized().x, 0.0f, move_in.normalized().y) * responsiveness + glm::vec3(0, rb.force.y, 0);
	}
	else rb.force = glm::vec3(-rb.velocity.x, 0, -rb.velocity.z) * responsiveness + glm::vec3(0, rb.force.y, 0);

	if (rb.position.y < 1.5f && !ready_to_jump) { // temporary collision detection - to be replaced later
		rb.position = glm::vec3(rb.position.x, 1.5f, rb.position.z);
		rb.velocity.y = 0.0f;
		rb.force = glm::vec3(rb.force.x, 0, rb.force.z);
		ready_to_jump = true;
		if (input_system::key_held[GLFW_KEY_SPACE]) jump();
	}

	rb.rotation = glm::rotate(rb.rotation, rot_speed * rot.x, glm::vec3(1.0f, 0.0f, 0.0f));

	rb.update();


	glm::vec3 dir = glm::toMat3(rb.rotation) * glm::vec3(0, 0, 1);

	renderer::V = glm::lookAt(rb.position, rb.position + dir, glm::vec3(0.0f, 1.0f, 0.0f));
}

void game::player::jump()
{
	if (ready_to_jump) {
		ready_to_jump = false;
		rb.velocity += glm::vec3(0, jump_force, 0);
		rb.force += physics::gravity * rb.mass;
	}
}
