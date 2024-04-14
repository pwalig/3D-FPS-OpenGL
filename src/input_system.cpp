#include "input_system.h"

#define ACTIONS 3

using std::vector;

vector<void(*)()>** key_events = nullptr;
vector<std::function<void()>>** key_events_f = nullptr;
bool* input_system::key_held = nullptr;

void input_system::key_callback(
	GLFWwindow* window,
	int key,
	int scancode,
	int action,
	int mod
) {
	input_system::call_events(key, action);
	if (action == GLFW_PRESS) input_system::key_held[key] = true;
	if (action == GLFW_RELEASE) input_system::key_held[key] = false;
}

void input_system::init_all()
{
	input_system::init_events();
	input_system::init_events_f();
	input_system::init_held();
}

void input_system::init_events()
{
	if (key_events != nullptr) { printf("initialize not possible key_events not null. free resources first"); return; }
	key_events = new vector<void(*)()>*[3];
	for (int i = 0; i < ACTIONS; ++i) {
		key_events[i] = new vector<void(*)()>[GLFW_KEY_LAST + 1];
	}
}

void input_system::init_events_f()
{
	if (key_events_f != nullptr) { printf("initialize not possible key_events_f not null. free resources first"); return; }
	key_events_f = new vector<std::function<void()>>*[ACTIONS];
	for (int i = 0; i < ACTIONS; ++i) {
		key_events_f[i] = new vector<std::function<void()>>[GLFW_KEY_LAST + 1];
	}
}

void input_system::init_held()
{
	if (key_held != nullptr) { printf("initialize not possible key_held not null. free resources first"); return; }
	input_system::key_held = new bool[GLFW_KEY_LAST + 1];
	for (int i = 0; i < GLFW_KEY_LAST + 1; ++i) {
		input_system::key_held[i] = false;
	}
}

void input_system::free_all()
{
	if (key_events != nullptr) {
		for (int i = 0; i < ACTIONS; ++i) {
			if (key_events[i] != nullptr) {
				delete[] key_events[i];
			}
		}
		delete[] key_events;
	}
	if (key_events_f != nullptr) {
		for (int i = 0; i < ACTIONS; ++i) {
			if (key_events_f[i] != nullptr) {
				delete[] key_events_f[i];
			}
		}
		delete[] key_events_f;
	}
	if (input_system::key_held != nullptr) delete[] input_system::key_held;
}

void input_system::subscribe(void(*_event)(), const int& key, const int& action)
{
	if (key_events == nullptr) input_system::init_events();
	if (key_events != nullptr) key_events[action][key].push_back(_event);
	else printf("could not initialize key_events\n");
}

void input_system::subscribe(std::function<void()> _event, const int& key, const int& action)
{
	if (key_events_f == nullptr) input_system::init_events_f();
	if (key_events_f != nullptr) key_events_f[action][key].push_back(_event);
	else printf("could not initialize key_events_f\n");
}

void input_system::call_events(const int& key, const int& action)
{
	for (auto _event : key_events[action][key]) {
		_event();
	}
	for (auto _event : key_events_f[action][key]) {
		_event();
	}
}
