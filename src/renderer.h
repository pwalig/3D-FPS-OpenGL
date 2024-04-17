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
	extern std::vector<scene_loader::Model> all_models; //vector of all of the models
	extern std::vector<scene_loader::Model> list_to_render;

	void render_textured(const glm::mat4& M, const float* const mesh, const float* const uv, const int& n, const GLuint& tex);
    void draw_each_object(std::vector<scene_loader::Model> models);
	void draw_cube(const glm::mat4& M);
	void draw_scene(GLFWwindow* window);
}
