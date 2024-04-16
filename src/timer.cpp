#include "timer.h"
#include <time_system.h>


void time::timer::update()
{
	if(!(this->_paused)) this->time -= time::delta_time;

	if (this->time <= 0.0f) {
		this->stop();
		this->events.call_events();
	}
}

time::timer::timer() : time(0.0f), _id(nullptr), _paused(false) {}

void time::timer::start(const float& time_)
{
	if (this->_id == nullptr) {
		this->time = time_;
		this->_paused = false;
		this->_id = time::timer_calls.subscribe(std::bind(&time::timer::update, this));
	}
	else printf("Could not start timer, timer already started at: %d\n", *this->_id);
}

void time::timer::pause()
{
	this->_paused = true;
}

void time::timer::resume()
{
	this->_paused = false;
}

void time::timer::stop()
{
	this->time = 0.0f;
	if (this->_id != nullptr) time::timer_calls.unsubscribe(*(this->_id));
}
