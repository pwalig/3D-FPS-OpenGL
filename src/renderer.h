#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "camera.h"


namespace renderer {
	extern renderer::camera active_camera;
	extern float global_fov;

	void draw_scene(GLFWwindow* window);
	
}
