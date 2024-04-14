#pragma once

#include <vector>

namespace input_system {
	extern std::vector<void(*)()>* key_pressed_events;
	extern std::vector<void(*)()>* key_released_events;
	extern bool* key_held;

	void initialize();
	void free();

	void subscribe(void(*_event)(), const int& key, const bool& pressed_released = true);
 }