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

	template <typename T, typename ...Args>
	T* instantiate(Args... args, const std::string& name = "script"); // function for creating script instances from other scripts

	void destroy(scripts_system::script* script);

	scripts_system::script* find_script(const std::string& name); // find script by name
	std::vector<scripts_system::script*> find_scripts(const std::string& name); // find all scripts with name
}

template <typename T, typename ...Args>
inline T* scripts_system::instantiate(Args... args, const std::string& name) {
	T* scr = new T(args...);
	scripts_system::scripts.push_back(scr);
	scr->name = name;
	scr->start();
	return scr;
}