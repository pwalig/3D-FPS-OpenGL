#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <scene_loader.h>
#include <vector>


namespace renderer {
	extern glm::mat4x4 V; //view matrix
	extern glm::mat4x4 P; //perspective matrix
	extern std::vector<scene_loader::Model> models; //models

	void render_textured(const glm::mat4& M, const float* const mesh, const float* const uv, const int& n, const GLuint& tex);
    void draw_each_object(std::vector<scene_loader::Model> models);
	void drawCube(const glm::mat4& M);
	void drawScene(GLFWwindow* window);
}
