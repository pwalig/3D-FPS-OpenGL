#pragma once
#include <event_subscription_list.h>
#include <object_subscription_list.h>
#include <game_script.h>

#define SCRIPTS_START 0
#define SCRIPTS_UPDATE 1
#define SCRIPTS_AMOUNT 2

namespace scripts_system {
	extern engine::event_subscription_list<>* events; //events[type].method();
	extern engine::object_subscription_list<scripts_system::script> scripts;

	void initialize();
	void update();
	void free();


	template <typename T, typename ...Args>
	T* instantiate(Args... args, const std::string& name = "script"); // function for creating script instances from other scripts

	template <typename T, typename ...Args>
	T* instantiate(Args... args, const scripts_system::script* const spawner, const std::string& name = "script"); // function for creating script instances from other scripts
	void _move_same_scene(const scripts_system::script* const spawner, scripts_system::script* scr); // helper function - don't call

	void destroy(scripts_system::script* script);
	void safe_destroy(scripts_system::script* script);

	scripts_system::script* find_script(const std::string& name); // find script by name
	std::vector<scripts_system::script*> find_scripts(const std::string& name); // find all scripts with name
}

template <typename T, typename ...Args>
inline T* scripts_system::instantiate(Args... args, const std::string& name) {
	T* scr = new T(args...);
	scripts_system::scripts.subscribe(scr);
	scr->name = name;
	scripts_system::events[SCRIPTS_START].subscribe(std::bind(&scripts_system::script::start, scr));
	return scr;
}

template <typename T, typename ...Args>
inline T* scripts_system::instantiate(Args... args, const scripts_system::script* const spawner, const std::string& name) {
	T* scr = scripts_system::instantiate<T, Args...>(args..., name);
	scripts_system::_move_same_scene(spawner, scr); // move to appropriate scene
	return scr;
}