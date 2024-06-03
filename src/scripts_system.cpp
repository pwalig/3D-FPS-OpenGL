#include "scripts_system.h"
#include <scene_loader.h>

engine::event_subscription_list<>* scripts_system::events = nullptr;

engine::object_subscription_list<scripts_system::script> scripts_system::scripts;

void scripts_system::initialize()
{
	if (events == nullptr) events = new engine::event_subscription_list<>[SCRIPTS_AMOUNT];
	else printf("Could not initialize, events was not nullptr, free resources first.\n");
}

void scripts_system::free()
{
	if (events != nullptr) delete[] events;
	scripts_system::scripts.perform_on_all([](scripts_system::script* scr) { scripts_system::destroy(scr); }); // delete un deleted scripts
	scripts_system::scripts.clear();
}

void scripts_system::update()
{
	events[SCRIPTS_START].call_events();
	events[SCRIPTS_START].clear();
	events[SCRIPTS_UPDATE].call_events();
	scripts_system::scripts.perform_on_all([](scripts_system::script* scr) { scr->update(); });
}

void scripts_system::destroy(scripts_system::script* script) {
	if (script) {
		if (scripts_system::scripts.on_list(script)) {
			try { 
				delete script;
				script = nullptr;
			} // delete script instance
			catch (...) {
				printf("Script deletion error: script was: %d\n", script);
			}
		}
		else printf("Script deletion error: script %d was unsubscribed\n", script);
	}
	else printf("Script deletion error: script was null\n");
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
	for (scripts_system::script* scr : scripts_system::scripts._objects)
		if (scr->name == name) return scr;
	return nullptr;
}

std::vector<scripts_system::script*> scripts_system::find_scripts(const std::string& name) {
	std::vector<scripts_system::script*> scrs;
	for (scripts_system::script* scr : scripts_system::scripts._objects)
		if (scr->name == name) scrs.push_back(scr);
	return scrs;
}
