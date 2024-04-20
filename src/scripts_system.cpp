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
	case SCRIPTS_INIT:
		for (auto script : scripts) script->init();
		break;
	case SCRIPTS_START:
		for (auto script : scripts) script->start();
		break;
	case SCRIPTS_UPDATE:
		for (auto script : scripts) script->update();
		break;
	case SCRIPTS_FREE:
		for (auto script : scripts) script->free();
		break;
	default:
		break;
	}
}
