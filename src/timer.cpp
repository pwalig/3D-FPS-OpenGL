#include "timer.h"
#include <time_system.h>


void time_system::timer::update()
{
	if(!(this->_paused)) this->time -= time_system::delta_time;

	if (this->time <= 0.0f) {
		this->stop();
		this->events.call_events();
	}
}

time_system::timer::timer() : time(0.0f), _id(nullptr), _paused(false) {}

void time_system::timer::start(const float& time_)
{
	if (this->_id == nullptr) {
		this->time = time_;
		this->_paused = false;
		this->_id = time_system::timer_calls.subscribe(std::bind(&time_system::timer::update, this));
	}
	else printf("Could not start timer, timer already started at: %d\n", *this->_id);
}

void time_system::timer::pause()
{
	this->_paused = true;
}

void time_system::timer::resume()
{
	this->_paused = false;
}

void time_system::timer::stop()
{
	this->time = 0.0f;
	if (this->_id != nullptr) time_system::timer_calls.unsubscribe(*(this->_id));
}

time_system::timer::~timer()
{
	stop();
}
