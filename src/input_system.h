#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <event_subscription_list.h>
#include <functional>

namespace input_system {
	extern bool* key_held; // read only
	extern engine::event_subscription_list<>** key_events; // key_events[action][key].method();

	//extern double last_mouse_x;
	//extern double last_mouse_y;
	extern glm::vec2 last_mouse;
	//extern double mouse_delta_x;
	//extern double mouse_delta_y;
	extern glm::vec2 mouse_delta;
	extern bool mouse_first_move;
	extern float mouse_sensitivity_multiplier;
	extern double global_mouse_sensitivity;

	void init_all();
	void init_events();
	void init_held();
	void free_all();


	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mod);
	void mouse_button_callback(GLFWwindow* window, int key, int action, int mods);
	void mouse_callback(GLFWwindow* window, double xpos, double ypos);
	void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
	void subscribe(void(*_event)(), const int& key, const int& action);
	void subscribe(std::function<void()> _event, const int& key, const int& action);

	void call_events(const int& key, const int& action);
 }