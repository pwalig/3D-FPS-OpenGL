#include "player_script.h"
#include <renderer.h>
#include <physics.h>
#include <input_system.h>
#include <axis.h>

namespace game {
	namespace player {
		physics::rigidbody game::player::rb;
		float max_speed = 7.0f;
		float responsiveness = 20.0f;
		float rot_speed = 0.03f;
	}
}

input_system::double_axis move = input_system::double_axis(input_system::axis(GLFW_KEY_A, GLFW_KEY_D), input_system::axis(GLFW_KEY_W, GLFW_KEY_S));

input_system::double_axis rot = input_system::double_axis(input_system::axis(GLFW_KEY_UP, GLFW_KEY_DOWN), input_system::axis(GLFW_KEY_LEFT, GLFW_KEY_RIGHT));

void game::player::start()
{
}

void game::player::movement()
{
	if (glm::length(rb.velocity) < max_speed && glm::length(move.normalized()) != 0.0f) {
		rb.force = glm::vec3(move.normalized().x, 0.0f, move.normalized().y) * responsiveness;
	}
	else rb.force = -rb.velocity * responsiveness;

	rb.angular_speed = glm::rotate(glm::mat4(1.0f), rot_speed * rot.normalized().y, glm::vec3(0.0f, 1.0f, 0.0f));
	rb.angular_speed = glm::rotate(rb.angular_speed, rot_speed * rot.normalized().x, glm::vec3(1.0f, 0.0f, 0.0f));

	physics::update(&rb);
}
