#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>


namespace renderer {
	struct model {
	public:
		glm::mat4 model_matrix;  // model matrix as a regular variable

		model(const glm::mat4& initialMatrix = glm::mat4(1.0f)); // Constructor (if you want to initialize the matrix during creation)
		~model();
	};

	extern glm::mat4x4 V; //view matrix
	extern glm::mat4x4 P; //perspective matrix
	extern std::vector<renderer::model*> all_models; //vector of all of the models

	

	void render_textured(const glm::mat4& M, const float* const mesh, const float* const uv, const int& n, const GLuint& tex);
    void draw_each_object(std::vector<renderer::model*> models);
	void draw_cube(const glm::mat4& M);
	void draw_scene(GLFWwindow* window);
}
