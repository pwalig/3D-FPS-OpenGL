#include "input_system.h"
#include <GLFW/glfw3.h>

using std::vector;

vector<void(*)()>* input_system::key_pressed_events = nullptr;
vector<void(*)()>* input_system::key_released_events = nullptr;
bool* input_system::key_held = nullptr;

void input_system::initialize()
{
	input_system::free();

	input_system::key_pressed_events = new vector<void(*)()>[GLFW_KEY_LAST + 1];
	input_system::key_released_events = new vector<void(*)()>[GLFW_KEY_LAST + 1];
	input_system::key_held = new bool[GLFW_KEY_LAST + 1];
}

void input_system::free()
{
	if (input_system::key_pressed_events != nullptr) delete[] input_system::key_pressed_events;
	if (input_system::key_released_events != nullptr) delete[] input_system::key_released_events;
	if (input_system::key_held != nullptr) delete[] input_system::key_held;
}

void input_system::subscribe(void(*_event)(), const int& key, const bool& pressed_released)
{
	if (pressed_released) key_pressed_events[key].push_back(_event);
	else key_released_events[key].push_back(_event);
}
