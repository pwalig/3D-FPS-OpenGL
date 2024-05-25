#include "terminal.h"
#include "scripts_system.h"
#include "input_system.h"
#include <glm/ext/matrix_transform.hpp>
#include <gameplay_manager.h>
#include "scene_loader.h"
#include "entity.h"

// function from: https://stackoverflow.com/questions/5888022/split-string-by-single-spaces
size_t split(const std::string& txt, std::vector<std::string>& strs, char ch)
{
	size_t pos = txt.find(ch);
	size_t initialPos = 0;
	strs.clear();

	// Decompose statement
	while (pos != std::string::npos) {
		strs.push_back(txt.substr(initialPos, pos - initialPos));
		initialPos = pos + 1;

		pos = txt.find(ch, initialPos);
	}

	// Add the last one
	strs.push_back(txt.substr(initialPos, std::min(pos, txt.size()) - initialPos + 1));

	return strs.size();
}

game::terminal* game::terminal::active_terminal = nullptr;
std::map<std::string, std::function<void(std::vector<std::string>)>> game::terminal::commands = {
	{"print", [](std::vector<std::string> args) { game::terminal::active_terminal->info_text.text += args[1]; }},
	{"load", [](std::vector<std::string> args) { scene_loader::load_scene(args[1]); }},
	{"unload", [](std::vector<std::string> args) { scene_loader::un_load_scene(args[1]); }},
	{"reset", [](std::vector<std::string> args) { *gameplay_manager::player_position = glm::vec3(0.0f, 3.0f, 0.0f); }},
	{"clear", [](std::vector<std::string> args) { game::terminal::active_terminal->info_text.text = ""; }},
	{"heal", [](std::vector<std::string> args) {
		game::entity* ent = scripts_system::find_script_of_type<game::entity>(args[1]);
		if (ent) {
			ent->heal(ent->max_hp);
			game::terminal::active_terminal->info_text.text += args[1] + " healed\n";
		}
	}}
};

void game::terminal::terminal_toggle()
{
	if (game::terminal::active_terminal) {
		for (int i = GLFW_KEY_SPACE; i <= GLFW_KEY_GRAVE_ACCENT; ++i) {
			input_system::key_events[GLFW_PRESS][i].unsubscribe(game::terminal::active_terminal->subscribes[i - GLFW_KEY_SPACE]);
		}
		input_system::key_events[GLFW_PRESS][GLFW_KEY_ENTER].unsubscribe(game::terminal::active_terminal->subscribes.back());
		game::terminal::active_terminal->subscribes.pop_back();
		input_system::key_events[GLFW_PRESS][GLFW_KEY_BACKSPACE].unsubscribe(game::terminal::active_terminal->subscribes.back());
		scripts_system::destroy(game::terminal::active_terminal);
		game::terminal::active_terminal = nullptr;
	}
	else {
		game::terminal::active_terminal = scripts_system::instantiate<game::terminal>("terminal"); // create new terminal
	}
}

game::terminal::terminal() : input_text("", "../assets/fonts/bitmap/pixel-mono.png", glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 40.0f, -1.0f)), glm::vec3(20.0f, 30.0f, 1.0f))),
	info_text("", "../assets/fonts/bitmap/pixel-mono.png", glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 1040.0f, -1.0f)), glm::vec3(20.0f, 30.0f, 1.0f))),
	fps_meter("fps: ", "../assets/fonts/bitmap/pixel-mono.png", glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 1080.0f, -10.0f)), glm::vec3(20.0f, 30.0f, 1.0f))) {

	// fps counter
	ft.function = [this]() {
		this->fps_meter.text = "fps: " + std::to_string(frames);
		this->frames = 0;
		ft.start(1.0f);
		};
	ft.start(1.0f);

	// text input
	for (int i = GLFW_KEY_SPACE; i <= GLFW_KEY_GRAVE_ACCENT; ++i) {
		subscribes.push_back(input_system::key_events[GLFW_PRESS][i].subscribe([this, i]() {
			char ch = (char)i;
			if (input_system::key_held[GLFW_KEY_RIGHT_SHIFT] || input_system::key_held[GLFW_KEY_LEFT_SHIFT]) {
				switch (i) {
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
			this->input_text.text += ch;
			}));
	}
	subscribes.push_back(input_system::key_events[GLFW_PRESS][GLFW_KEY_BACKSPACE].subscribe([this]() { 
		if (input_system::key_held[GLFW_KEY_LEFT_CONTROL]) this->input_text.text = ""; // erase everything with ctrl+backspace
		else if (!this->input_text.text.empty()) this->input_text.text.pop_back();
		}));
	subscribes.push_back(input_system::key_events[GLFW_PRESS][GLFW_KEY_ENTER].subscribe([this]() { this->process_command(); }));
}

void game::terminal::update()
{
	++frames;
}

void game::terminal::process_command() {
	std::vector<std::string> args;
	split(this->input_text.text, args, ' ');
	if (game::terminal::commands.find(args[0]) != game::terminal::commands.end()) game::terminal::commands[args[0]](args);
	else {
		game::terminal::active_terminal->info_text.text += args[0] + " no such command\n";
		game::terminal::active_terminal->info_text.text += "available commands are:\n";
		for (auto it : game::terminal::commands) game::terminal::active_terminal->info_text.text += it.first + "\n";
	}
	this->input_text.text = "";
}