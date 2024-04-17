#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>

namespace renderer {
	extern glm::mat4x4 V; //view matrix
	extern glm::mat4x4 P; //perspective matrix

	void render_textured(const glm::mat4& M, const float* const mesh, const float* const uv, const int& n, const GLuint& tex);
	void drawCube(const glm::mat4& M);
}