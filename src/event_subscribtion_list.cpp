#include "event_subscribtion_list.h"

void engine::event_subscribtion_list::call_events() const
{
	for (auto _event : this->_events) {
		_event();
	}
	for (auto _event : this->_events_f) {
		_event();
	}
}

int engine::event_subscribtion_list::subscribe(void(*_event)()) {
	this->_events.push_back(_event);
	return _events.size() - 1;
}

int engine::event_subscribtion_list::subscribe(std::function<void()> _event) {
	this->_events_f.push_back(_event);
	return _events.size() - 1;
}

void engine::event_subscribtion_list::unsubscribe(void(*_event)()) {
	remove(this->_events.begin(), this->_events.end(), _event);
}

void engine::event_subscribtion_list::unsubscribe(void(*_event)(), const int& id) {
	this->_events.erase(this->_events.begin() + id);
}

void engine::event_subscribtion_list::unsubscribe(std::function<void()> _event, const int& id) {
	//remove(this->_events_f.begin(), this->_events_f.end(), _event); // didn't work
	this->_events_f.erase(this->_events_f.begin() + id);
}
