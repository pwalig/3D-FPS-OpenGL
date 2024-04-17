#pragma once

#include <GLFW/glfw3.h>
#include <event_subscription_list.h>
#include <functional>

namespace input_system {
	extern bool* key_held; // read only
	extern engine::event_subscribtion_list** key_events; // key_events[action][key].method();

	void init_all();
	void init_events();
	void init_held();

	void free_all();


	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mod);

	void subscribe(void(*_event)(), const int& key, const int& action);
	void subscribe(std::function<void()> _event, const int& key, const int& action);

	void call_events(const int& key, const int& action);
 }