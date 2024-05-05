#include "key_bind.h"
#include "input_system.h"

input_system::key_bind::key_bind(const std::function<void()>& function, const int& key, const int& action) : _action(action), _key(key)
{
	this->_id = input_system::key_events[_action][_key].subscribe(function);
}

input_system::key_bind::~key_bind()
{
	input_system::key_events[_action][_key].unsubscribe(_id);
}
