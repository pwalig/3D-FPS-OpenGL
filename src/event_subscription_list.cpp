#include "event_subscription_list.h"

void engine::event_subscribtion_list::call_events() const
{
	for (auto it = this->_events.begin(); it != this->_events.end(); it < this->_events.end() ? ++it : it+=0 ) {
		const auto d = std::distance(_events.begin(), it);
		it->_event();
		it = _events.begin();
		std::advance(it, d);
	}
}

int* engine::event_subscribtion_list::subscribe(const std::function<void()>& _event) {
	int id = _events.size();
	this->_events.push_back(record(_event, id));
	return this->_events.back().id;
}

void engine::event_subscribtion_list::unsubscribe(int id) {
	this->_events.erase(this->_events.begin() + id);
	if (_events.size() > 0) {
		for (auto it = this->_events.begin() + id; it != this->_events.end(); ++it) {
			--(*it->id);
		}
	}
}



// engine::record

engine::record::record(const std::function<void()>& event_, const int& id) : _event(event_), id(new int(id)) {}

engine::record::record(const record& other) : _event(other._event), id(new int(*(other.id))) {}

engine::record& engine::record::operator=(const record& other)
{
	if (this != &other) {
		delete (this->id);

		this->_event = other._event;
		this->id = new int(*(other.id));
	}
	return *this;
}

engine::record::record(record&& other) noexcept : _event(other._event), id(other.id)
{
	other.id = nullptr;
}

engine::record& engine::record::operator=(record&& other) noexcept
{
	if (this != &other)
	{
		delete (this->id);

		this->_event = other._event;
		this->id = other.id;
	}
	other.id = nullptr;
	return *this;
}

engine::record::~record()
{
	delete (this->id);
}
