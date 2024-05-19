#include "ui_system.h"
#include "engine.h"
#include <glm/ext/matrix_clip_space.hpp>


glm::mat4 ui_system::P = glm::ortho(0.0f, 1920.0f, 0.0f, 1080.0f, 0.1f, 1000.0f);


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
