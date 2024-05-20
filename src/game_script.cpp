#include "game_script.h"
#include "scripts_system.h"

scripts_system::script::script(const std::string& name_) : name(name_) {
	scripts_system::scripts.subscribe(this);
	scripts_system::events[SCRIPTS_START].subscribe(std::bind(&scripts_system::script::start, this));
}

scripts_system::script::~script()
{
	scripts_system::scripts.unsubscribe(this); // erase script from subscription list
}
