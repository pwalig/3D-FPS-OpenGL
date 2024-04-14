#include "engine.h"
#include <vector>

double engine::delta_time;
double engine::time_scale = 1.0;

std::vector<void(*)()>* events = nullptr;
std::vector<std::function<void()>>* events_f = nullptr;
std::vector <engine::script*> scripts;

void engine::initialize()
{
	engine::free();
	events = new std::vector<void(*)()>[ENGINE_AT_AMOUNT];
	events_f = new std::vector<std::function<void()>>[ENGINE_AT_AMOUNT];
}

void engine::free()
{
	if (events != nullptr) delete[] events;
	if (events_f != nullptr) delete[] events_f;
}

void engine::subscribe(void(*_event)(), const int& type) { events[type].push_back(_event); }

void engine::subscribe(std::function<void()> _event, const int& type) { events_f[type].push_back(_event); }

void engine::subscribe(script* script) { scripts.push_back(script); }

void engine::call_events(const int& type)
{
	for (auto _event : events[type]) {
		_event();
	}
	for (auto _event : events_f[type]) {
		_event();
	}
	switch (type)
	{
	case ENGINE_AT_INIT:
		for (auto script : scripts) if (script->active) script->init();
		break;
	case ENGINE_AT_START:
		for (auto script : scripts) if (script->active) script->start();
		break;
	case ENGINE_AT_UPDATE:
		for (auto script : scripts) if (script->active) script->update();
		break;
	case ENGINE_AT_FREE:
		for (auto script : scripts) if (script->active) script->free();
		break;
	default:
		break;
	}
}
