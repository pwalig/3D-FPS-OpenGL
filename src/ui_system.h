#pragma once
#include <glm/glm.hpp>

namespace ui_system {
	extern glm::mat4 P;

	namespace quad {
		extern const int vertex_count;
		extern const float vertices[24];
		extern const float texture_coordinates[12];
	}
}