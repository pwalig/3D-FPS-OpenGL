#include "timer.h"
#include <time_system.h>


void time::timer::update()
{
	if(!(this->_paused)) this->time -= time::delta_time;
	printf("%f\n", this->time);

	if (this->time <= 0.0f) {
		this->stop();
		this->events.call_events();
	}
}

time::timer::timer() : time(0.0f), _id(-1), _paused(false) {}

void time::timer::start(const float& time_)
{
	if (_id == -1) {
		this->time = time_;
		this->_paused = false;
		this->_id = time::timer_calls.subscribe(std::bind(&time::timer::update, this));
		printf("start id: %d\n", this->_id);
	}
	else printf("Could not start timer, timer already started\n");
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
	printf("stop id: %d\n", this->_id);
	if (_id != -1) time::timer_calls.unsubscribe(std::bind(&time::timer::update, this), this->_id);
	this->_id = -1;
}

time::timer::~timer()
{
	if (_id != -1) time::timer_calls.unsubscribe(std::bind(&time::timer::update, this), this->_id);
}
