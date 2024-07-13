#include <debugger.h>
#ifdef DEBUG
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

float game::debugger::scripts_time = 0.0f;
float game::debugger::timers_time = 0.0f;
float game::debugger::physics_time = 0.0f;
float game::debugger::render_time = 0.0f;
float game::debugger::callbacks_time = 0.0f;

game::debugger* game::debugger::active_debugger = nullptr;
std::map<std::string, std::function<void(std::vector<std::string>)>> game::debugger::commands = {
	{"print", [](std::vector<std::string> args) { game::debugger::active_debugger->info_text.text += args[1]; }},
	{"load", [](std::vector<std::string> args) { scene_loader::load_scene(args[1]); }},
	{"unload", [](std::vector<std::string> args) { scene_loader::un_load_scene(args[1]); }},
	{"reset", [](std::vector<std::string> args) { *gameplay_manager::player_position = glm::vec3(0.0f, 3.0f, 0.0f); }},
	{"tp", [](std::vector<std::string> args) { *gameplay_manager::player_position = glm::vec3(stof(args[1]), stof(args[2]), stof(args[3])); }},
	{"clear", [](std::vector<std::string> args) { game::debugger::active_debugger->info_text.text = ""; }},
	{"heal", [](std::vector<std::string> args) {
		game::entity* ent = scripts_system::find_script_of_type<game::entity>(args[1]);
		if (ent) {
			ent->heal(ent->max_hp);
			game::debugger::active_debugger->info_text.text += args[1] + " healed\n";
		}
	}}
};

game::debugger::debugger() :
	input_text("", "../assets/fonts/bitmap/handwiriting-readable.png", glm::vec3(0.0f, 0.05f, 0.015f), glm::scale(glm::mat4(1.0f), glm::vec3(0.017f, 0.03f, 1.0f)), glm::vec3(-1.0f, -1.0f, 0.0f)),
	info_text(" ", "../assets/fonts/bitmap/handwiriting-readable.png", glm::vec3(0.0f, 0.7, 0.015f), glm::scale(glm::mat4(1.0f), glm::vec3(0.017f, 0.03f, 1.0f)), glm::vec3(-1.0f, -1.0f, 0.0f)),
	fps_meter("fps: ", "../assets/fonts/bitmap/handwiriting-readable.png", glm::vec3(0.0f, 1.0f, 0.015f), glm::scale(glm::mat4(1.0f), glm::vec3(0.017f, 0.03f, 1.0f)), glm::vec3(-1.0f, -1.0f, 0.0f)) {

	// fps counter
	ft.function = [this]() {
		this->fps_meter.text = "fps: " + std::to_string((float)frames * 2.0f)
			+ "\nscripts time: " + std::to_string(game::debugger::scripts_time / (float)(this->frames))
			+ "\ntimers time: " + std::to_string(game::debugger::timers_time / (float)(this->frames))
			+ "\nphysics time: " + std::to_string(game::debugger::physics_time / (float)(this->frames))
			+ "\nrender time: " + std::to_string(game::debugger::render_time / (float)(this->frames))
			+ "\ncallbacks time: " + std::to_string(game::debugger::callbacks_time / (float)(this->frames));
		this->frames = 0;
		game::debugger::scripts_time = 0.0f;
		game::debugger::timers_time = 0.0f;
		game::debugger::physics_time = 0.0f;
		game::debugger::render_time = 0.0f;
		game::debugger::callbacks_time = 0.0f;
		ft.start(0.5f);
		};
	ft.start(0.5f);
}

void game::debugger::update()
{
	++frames;
}

void game::debugger::toggle_terminal()
{
	if (input_system::active_text_field != &(this->input_text)) input_system::active_text_field = &(this->input_text);
	else if (input_system::active_text_field == &(this->input_text)) {
		input_system::active_text_field = nullptr;
		this->input_text.text = "";
	}
}

void game::debugger::process_command() {
	std::vector<std::string> args;
	split(this->input_text.text, args, ' ');
	if (game::debugger::commands.find(args[0]) != game::debugger::commands.end()) game::debugger::commands[args[0]](args);
	else {
		game::debugger::active_debugger->info_text.text += args[0] + " no such command\n";
		game::debugger::active_debugger->info_text.text += "available commands are:\n";
		for (auto it : game::debugger::commands) game::debugger::active_debugger->info_text.text += it.first + "\n";
	}
	this->input_text.text = "";
}

void game::debugger::toggle()
{
	if (game::debugger::active_debugger) {
		input_system::active_text_field = nullptr;
		scripts_system::destroy(game::debugger::active_debugger);
		game::debugger::active_debugger = nullptr;
	}
	else {
		game::debugger::active_debugger = scripts_system::instantiate<game::debugger>("terminal"); // create new terminal
	}
}

#endif