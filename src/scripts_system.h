#pragma once
#include <event_subscription_list.h>
#include <game_script.h>

#define SCRIPTS_INIT 0
#define SCRIPTS_START 1
#define SCRIPTS_UPDATE 2
#define SCRIPTS_FREE 3
#define SCRIPTS_AMOUNT 4

namespace scripts_system {
	extern engine::event_subscription_list<>* events; //events[type].method();
	extern std::vector <scripts_system::script*> scripts;

	void initialize();
	void free();

	void call_events(const int& type);
}