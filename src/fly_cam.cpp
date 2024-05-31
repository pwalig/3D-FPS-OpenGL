#include "fly_cam.h"
#include <renderer.h>

void game::fly_cam::start()
{
	renderer::active_camera.set_V(position, rotation);
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

	renderer::active_camera.set_V(position, rotation);
}
