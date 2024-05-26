#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>


namespace renderer {
	extern glm::mat4x4 V; //view matrix
	extern glm::mat4x4 P; //perspective matrix

	void draw_scene(GLFWwindow* window);
	
}
