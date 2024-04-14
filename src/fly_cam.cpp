#include "fly_cam.h"
#include <renderer.h>
#include <glm/ext/matrix_transform.hpp>
#include <input_system.h>
#include <engine.h>
#include <glm/ext/matrix_clip_space.hpp>

namespace game {
	namespace fly_cam {
		glm::vec3 pos = glm::vec3(0, 1, -5);
		glm::vec3 dir = glm::vec3(0, 0, 1);
		float mov_speed = 2.0f;
		float rot_speed = 0.5f;
	}
}

void game::fly_cam::start()
{
	renderer::V = glm::lookAt(pos, pos + dir, glm::vec3(0.0f, 1.0f, 0.0f));
	renderer::P = glm::perspective(glm::radians(70.0f), 1.0f, 1.0f, 50.0f);
}

float angle_y = 0;
float angle_x = 0;

void game::fly_cam::update()
{
	float up_down = 0.0f;
	if (input_system::key_held[GLFW_KEY_UP]) up_down -= rot_speed;
	if (input_system::key_held[GLFW_KEY_DOWN])up_down += rot_speed;
	angle_x += up_down * engine::delta_time;

	float left_right = 0.0f;
	if (input_system::key_held[GLFW_KEY_LEFT]) left_right += rot_speed;
	if (input_system::key_held[GLFW_KEY_RIGHT]) left_right -= rot_speed;
	angle_y += left_right * engine::delta_time;

	float forward_back = 0.0f;
	if (input_system::key_held[GLFW_KEY_W]) forward_back += mov_speed;
	if (input_system::key_held[GLFW_KEY_S]) forward_back -= mov_speed;

	float side_to_side = 0.0f;
	if (input_system::key_held[GLFW_KEY_A]) side_to_side -= mov_speed;
	if (input_system::key_held[GLFW_KEY_D]) side_to_side += mov_speed;

	float elevate = 0.0f;
	if (input_system::key_held[GLFW_KEY_Q]) elevate -= mov_speed;
	if (input_system::key_held[GLFW_KEY_E]) elevate += mov_speed;

	glm::mat4 Mc = rotate(glm::mat4(1.0f), angle_y, glm::vec3(0, 1, 0));
	Mc = rotate(Mc, angle_x, glm::vec3(1, 0, 0));
	glm::vec4 dir_ = Mc * glm::vec4(0, 0, 1, 0);
	dir = glm::normalize(glm::vec3(dir_));

	pos += dir * forward_back * (float)engine::delta_time;
	pos += glm::normalize(glm::cross(dir, glm::vec3(0.0f, 1.0f, 0.0f))) * side_to_side * (float)engine::delta_time;
	pos += glm::vec3(0.0f, elevate * engine::delta_time, 0.0f);

	renderer::V = glm::lookAt(pos, pos + dir, glm::vec3(0.0f, 1.0f, 0.0f));
}
