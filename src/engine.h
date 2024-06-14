#pragma once
#include <GLFW/glfw3.h> 

namespace engine {	
	extern GLFWwindow* window;
	extern float window_width;
	extern float window_height;
	extern float width;
	extern float height;
	extern bool is_fullscreen;
	extern int windowed_xpos;
	extern int windowed_ypos;
}