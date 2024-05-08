#include "scripts_system.h"

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
}

void scripts_system::call_events(const int& type)
{
	events[type].call_events();
	switch (type)
	{
	case SCRIPTS_START:
		for (auto script : scripts) script->start();
		break;
	case SCRIPTS_UPDATE:
		for (auto script : scripts) script->update();
		break;
	default:
		break;
	}
}

void scripts_system::destroy(scripts_system::script* script) {
	std::vector<scripts_system::script*>::iterator id = std::find(scripts_system::scripts.begin(), scripts_system::scripts.end(), script); // find script in vector
	if (id != scripts_system::scripts.end()) scripts_system::scripts.erase(id); // erase script from vector
	delete script; // delete script instance
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
