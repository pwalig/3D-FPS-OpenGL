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

time_system::timer::timer() : time(0.0f), _paused(false) {}

void time_system::timer::start(const float& time_)
{
	if (!time_system::timers.on_list(this)) {
		this->time = time_;
		this->_paused = false;
		time_system::timers.subscribe(this);
	}
	else printf("Could not start timer, timer already started.\n");
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
	time_system::timers.unsubscribe(this);
}

time_system::timer::~timer()
{
	stop();
}
