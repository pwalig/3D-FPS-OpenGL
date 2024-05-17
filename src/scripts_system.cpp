#include "scripts_system.h"
#include <scene_loader.h>

engine::event_subscription_list<>* scripts_system::events = nullptr;

std::vector <scripts_system::script*> scripts_system::scripts;

void scripts_system::initialize()
{
	if (events == nullptr) events = new engine::event_subscription_list<>[SCRIPTS_AMOUNT];
	else printf("Could not initialize, events was not nullptr, free resources first.\n");
}

void scripts_system::free()
{
	if (events != nullptr) delete[] events;
	for (scripts_system::script* i : scripts_system::scripts) { // delete un deleted scripts
		scripts_system::destroy(i);
	}
	scripts_system::scripts.clear();
}

void scripts_system::update()
{
	events[SCRIPTS_START].call_events();
	events[SCRIPTS_START].clear();
	events[SCRIPTS_UPDATE].call_events();
	for (auto script : scripts) script->update();
}

void scripts_system::destroy(scripts_system::script* script) {
	std::vector<scripts_system::script*>::iterator id = std::find(scripts_system::scripts.begin(), scripts_system::scripts.end(), script); // find script in vector
	if (id != scripts_system::scripts.end()) scripts_system::scripts.erase(id); // erase script from vector
	delete script; // delete script instance
}

void scripts_system::safe_destroy(scripts_system::script* script) {
	scripts_system::events[SCRIPTS_START].subscribe(std::bind(&scripts_system::destroy, script));
}

void scripts_system::_move_same_scene(const scripts_system::script* const spawner, scripts_system::script* scr)
{
	std::string scene = scene_loader::get_scene_name(spawner);
	if (scene != "no_scene") scene_loader::open_scenes[scene].push_back(scr);
}

scripts_system::script* scripts_system::find_script(const std::string& name) {
	for (scripts_system::script* scr : scripts_system::scripts)
		if (scr->name == name) return scr;
	return nullptr;
}

std::vector<scripts_system::script*> scripts_system::find_scripts(const std::string& name) {
	std::vector<scripts_system::script*> scrs;
	for (scripts_system::script* scr : scripts_system::scripts)
		if (scr->name == name) scrs.push_back(scr);
	return scrs;
}
