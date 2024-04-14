#include "fly_cam.h"
#include <renderer.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <input_system.h>
#include <engine.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <axis.h>
#include "constants.h"

namespace game {
	namespace fly_cam {
		glm::vec3 position = glm::vec3(0, 1, -5);
		glm::quat rotation = glm::quat(glm::vec3(0, 0, 0));

		float speed = 0.2f;

		float rot_speed = PI * 0.05f;
		float max_rot = PI * 2.3f;
		glm::vec2 rot2 = glm::vec2(0.0f);

		input_system::triple_axis move_in = input_system::triple_axis(GLFW_KEY_A, GLFW_KEY_D, GLFW_KEY_E, GLFW_KEY_Q, GLFW_KEY_W, GLFW_KEY_S);
		input_system::double_axis rot_in = input_system::double_axis(GLFW_KEY_DOWN, GLFW_KEY_UP, GLFW_KEY_LEFT, GLFW_KEY_RIGHT);
	}
}

void game::fly_cam::start()
{
	renderer::V = glm::lookAt(position, position + (glm::toMat3(rotation) * glm::vec3(0, 0, 1)), glm::vec3(0.0f, 1.0f, 0.0f));
}

void game::fly_cam::update()
{
	rot2 += rot_in.normalized() * rot_speed;
	if (rot2.x > max_rot) rot2.x = max_rot;
	if (rot2.x < -max_rot) rot2.x = -max_rot;

	rotation = glm::rotate(glm::quat(glm::vec3(0, 0, 0)), rot_speed * rot2.y, glm::vec3(0.0f, 1.0f, 0.0f));
	rotation = glm::rotate(rotation, rot_speed * rot2.x, glm::vec3(1.0f, 0.0f, 0.0f));
	rotation = glm::normalize(rotation);

	position += (rotation * move_in.normalized()) * speed;

	renderer::V = glm::lookAt(position, position + glm::toMat3(rotation) * glm::vec3(0, 0, 1), glm::vec3(0.0f, 1.0f, 0.0f));
}
