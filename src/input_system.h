#pragma once

#include <vector>
#include <GLFW/glfw3.h>

namespace input_system {
	extern bool* key_held;

	void initialize();
	void free();

	void subscribe(void(*_event)(), const int& key, const int& action = GLFW_PRESS);

	void call_events(const int& key, const int& action);
 }