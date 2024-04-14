#pragma once

#include <GLFW/glfw3.h>
#include <vector>
#include <functional>

namespace input_system {
	extern bool* key_held;

	void initialize();
	void free();


	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mod);

	void subscribe(void(*_event)(), const int& key, const int& action = GLFW_PRESS);
	void subscribe(std::function<void()> _event, const int& key, const int& action = GLFW_PRESS);

	void call_events(const int& key, const int& action);
 }