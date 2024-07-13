#pragma once
#include <glm/glm.hpp>

namespace ui_system {
	extern glm::mat4 P;
	extern float scaling; // 0.0f full height - 1.0 full width

	namespace quad {
		extern const int vertex_count;
		extern const float vertices[24];
		extern const float texture_coordinates[12];
	}

	glm::vec2 to_ui_space(const glm::vec2& pos);
}