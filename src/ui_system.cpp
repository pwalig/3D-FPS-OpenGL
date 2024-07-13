#include "ui_system.h"
#include "engine.h"
#include <glm/ext/matrix_clip_space.hpp>

float ui_system::scaling = 0.0f;
glm::mat4 ui_system::P = glm::ortho(0.0f, engine::width, 0.0f, engine::height, 0.01f, 1.0f);

const int ui_system::quad::vertex_count = 6;
const float ui_system::quad::vertices[24] = {
	-1.0f, -1.0f, 0.0f, 1.0f,
	1.0f, -1.0f, 0.0f, 1.0f,
	-1.0f, 1.0f, 0.0f, 1.0f,

	1.0f, 1.0f, 0.0f, 1.0f,
	1.0f, -1.0f, 0.0f, 1.0f,
	-1.0f, 1.0f, 0.0f, 1.0f
};
const float ui_system::quad::texture_coordinates[12] = {
	0.0f, 0.0f,
	1.0f, 0.0f,
	0.0f, 1.0f,

	1.0f, 1.0f,
	1.0f, 0.0f,
	0.0f, 1.0f
};

glm::vec2 ui_system::to_ui_space(const glm::vec2& pos)
{
	return glm::vec2(
		pos.x / engine::width,
		1.0f - (pos.y / engine::height)
	);
}
