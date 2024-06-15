#include "input_system.h"
#include <engine.h>
#include <ui_button.h>

#define ACTIONS 3

engine::event_subscription_list<>** input_system::key_events = nullptr;
bool* input_system::key_held = nullptr;

ui_system::ui_text* input_system::active_text_field = nullptr;

glm::vec2  input_system::last_mouse = glm::vec2(0.0,0.0);
glm::vec2  input_system::mouse_delta = glm::vec2(0.0, 0.0);

bool input_system::mouse_first_move = true;
double input_system::global_mouse_sensitivity = 0.001;
float input_system::mouse_sensitivity_multiplier = 1.0;


void input_system::key_callback(
	GLFWwindow* window,
	int key,
	int scancode,
	int action,
	int mod
) {
	if (input_system::active_text_field && action == GLFW_PRESS) {
		if (key >= GLFW_KEY_SPACE && key <= GLFW_KEY_GRAVE_ACCENT) {
			char ch = (char)key;
			if (input_system::key_held[GLFW_KEY_RIGHT_SHIFT] || input_system::key_held[GLFW_KEY_LEFT_SHIFT]) {
				switch (key) {
				case GLFW_KEY_APOSTROPHE:
					ch = '"';
					break;
				case GLFW_KEY_COMMA:
					ch = '<';
					break;
				case GLFW_KEY_MINUS:
					ch = '_';
					break;
				case GLFW_KEY_PERIOD:
					ch = '>';
					break;
				case GLFW_KEY_SLASH:
					ch = '?';
					break;
				case GLFW_KEY_0:
					ch = ')';
					break;
				case GLFW_KEY_1:
					ch = '!';
					break;
				case GLFW_KEY_2:
					ch = '@';
					break;
				case GLFW_KEY_3:
					ch = '#';
					break;
				case GLFW_KEY_4:
					ch = '$';
					break;
				case GLFW_KEY_5:
					ch = '%';
					break;
				case GLFW_KEY_6:
					ch = '^';
					break;
				case GLFW_KEY_7:
					ch = '&';
					break;
				case GLFW_KEY_8:
					ch = '*';
					break;
				case GLFW_KEY_9:
					ch = '(';
					break;
				case GLFW_KEY_SEMICOLON:
					ch = ':';
					break;
				case GLFW_KEY_EQUAL:
					ch = '+';
					break;
				case GLFW_KEY_LEFT_BRACKET:
					ch = '{';
					break;
				case GLFW_KEY_RIGHT_BRACKET:
					ch = '}';
					break;
				case GLFW_KEY_BACKSLASH:
					ch = '|';
					break;
				case GLFW_KEY_GRAVE_ACCENT:
					ch = '~';
					break;
				}
			}
			else {
				if (ch >= 'A' && ch <= 'Z') {
					ch += 32;
				}
			}
			input_system::active_text_field->text += ch;
		}
		else if (key == GLFW_KEY_BACKSPACE) {
			if (mod && GLFW_MOD_CONTROL) input_system::active_text_field->text = ""; // erase everything with ctrl+backspace
			else if (!input_system::active_text_field->text.empty()) input_system::active_text_field->text.pop_back();
		}
	}
	input_system::call_events(key, action);
	if (action == GLFW_PRESS) input_system::key_held[key] = true;
	if (action == GLFW_RELEASE) input_system::key_held[key] = false;
}
void input_system::mouse_button_callback(GLFWwindow* window, int key, int action, int mods)
{
	input_system::call_events(key, action);
	if (action == GLFW_PRESS) input_system::key_held[key] = true;
	if (action == GLFW_RELEASE) input_system::key_held[key] = false;

	if (key == GLFW_MOUSE_BUTTON_1) {
		if (action == GLFW_PRESS) {
			ui_system::ui_button::mouse_hovered.perform_on_all([](ui_system::ui_button* uib) {
				if (!(uib->held)) {
					uib->on_click.call_events();
					uib->held = true;
				}
				});
		}
		else if (action == GLFW_RELEASE) {
			ui_system::ui_button::mouse_hovered.perform_on_all([](ui_system::ui_button* uib) {
				if (uib->held) { // to prevent calling release when: LMB was pressed before hovering
					uib->on_release.call_events();
					uib->held = false;
				}
				});
		}
	}
}

void input_system::mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	if (mouse_first_move) {
		last_mouse[0] = xpos;
		last_mouse[1] = ypos;
		mouse_first_move = false;
	}

	mouse_delta[0] = -1 * (xpos - last_mouse[0]) * global_mouse_sensitivity * mouse_sensitivity_multiplier;
	mouse_delta[1] = (ypos - last_mouse[1]) * global_mouse_sensitivity * mouse_sensitivity_multiplier;
	last_mouse[0] = xpos;
	last_mouse[1] = ypos;

	ui_system::ui_button::check_for_mouse_collisions(
		glm::vec2((xpos / engine::width) * 1920.0f, 1080.0f - ((ypos / engine::height) * 1080.0f))
	);
}

void input_system::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (xoffset > 0.0) for(int i = (int)xoffset; i > 0; --i) input_system::call_events(GLFW_MOUSE_BUTTON_LAST + 1, GLFW_PRESS);
	if (xoffset < 0.0) for(int i = (int)xoffset; i < 0; ++i) input_system::call_events(GLFW_MOUSE_BUTTON_LAST + 1, GLFW_RELEASE);
	if (yoffset > 0.0) for(int i = (int)yoffset; i > 0; --i) input_system::call_events(GLFW_MOUSE_BUTTON_LAST + 2, GLFW_PRESS);
	if (yoffset < 0.0) for(int i = (int)yoffset; i < 0; ++i) input_system::call_events(GLFW_MOUSE_BUTTON_LAST + 2, GLFW_RELEASE);
}



void input_system::init_all()
{
	input_system::init_events();
	input_system::init_held();
}

void input_system::init_events()
{
	if (key_events != nullptr) { printf("initialize not possible key_events not null. free resources first\n"); return; }
	key_events = new engine::event_subscription_list<>*[3];
	for (int i = 0; i < ACTIONS; ++i) {
		key_events[i] = new engine::event_subscription_list<>[GLFW_KEY_LAST + 1];
	}
}

void input_system::init_held()
{
	if (key_held != nullptr) { printf("initialize not possible key_held not null. free resources first\n"); return; }
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

	if (input_system::key_held != nullptr) delete[] input_system::key_held;
}

void input_system::subscribe(void(*_event)(), const int& key, const int& action)
{
	if (key_events == nullptr) input_system::init_events();
	if (key_events != nullptr) key_events[action][key].subscribe(_event);
	else printf("could not initialize key_events\n");
}

void input_system::subscribe(std::function<void()> _event, const int& key, const int& action)
{
	if (key_events == nullptr) input_system::init_events();
	if (key_events != nullptr) key_events[action][key].subscribe(_event);
	else printf("could not initialize key_events_f\n");
}

void input_system::call_events(const int& key, const int& action)
{
	key_events[action][key].call_events();
}
