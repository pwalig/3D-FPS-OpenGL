#include "scripts_system.h"

engine::event_subscribtion_list* scripts_system::events = nullptr;

namespace scripts_system {
	std::vector <scripts_system::script*> scripts;
}

void scripts_system::initialize()
{
	if (events == nullptr) events = new engine::event_subscribtion_list[SCRIPTS_AMOUNT];
	else printf("Could not initialize, events was not nullptr, free resources first.\n");
}

void scripts_system::free()
{
	if (events != nullptr) delete[] events;
}

void scripts_system::subscribe(void(*_event)(), const int& type) { events[type].subscribe(_event); }

void scripts_system::subscribe(std::function<void()> _event, const int& type) { scripts_system::events[type].subscribe(_event); }

void scripts_system::subscribe(scripts_system::script* script) { scripts_system::scripts.push_back(script); }

void scripts_system::call_events(const int& type)
{
	events[type].call_events();
	switch (type)
	{
	case SCRIPTS_INIT:
		for (auto script : scripts) if (script->active) script->init();
		break;
	case SCRIPTS_START:
		for (auto script : scripts) if (script->active) script->start();
		break;
	case SCRIPTS_UPDATE:
		for (auto script : scripts) if (script->active) script->update();
		break;
	case SCRIPTS_FREE:
		for (auto script : scripts) if (script->active) script->free();
		break;
	default:
		break;
	}
}
