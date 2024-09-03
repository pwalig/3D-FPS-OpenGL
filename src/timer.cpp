#include "timer.h"
#include <time_system.h>


// TIMER

void time_system::timer::update()
{
	if(!(this->pause)) this->time -= time_system::delta_time;

	if (this->time <= 0.0f) {
		this->stop();
		this->at_finish();
		if (this->die_on_finish) delete this;
	}
}

time_system::timer::timer() : time(0.0f), pause(false), die_on_finish(false) {}

time_system::timer::timer(const float& time_) : timer()
{
	this->start(time_);
}

void time_system::timer::start(const float& time_)
{
	if (!time_system::timers.on_list(this)) {
		this->time = time_;
		time_system::timers.subscribe(this);
	}
#ifdef _DEBUG
	else printf("Could not start timer, timer already started.\n");
#endif
}

void time_system::timer::stop()
{
	this->time = 0.0f;
	time_system::timers.unsubscribe(this);
}

time_system::timer::~timer()
{
	time_system::timers.unsubscribe(this);
}


// FUNCTION TIMER

time_system::function_timer::function_timer(const std::function<void()>& function_) : timer(), function(function_) {}

time_system::function_timer::function_timer(const float& time_, const std::function<void()>& function_) : timer(time_), function(function_) {}

void time_system::function_timer::at_finish()
{
	this->function();
}


// EVENTS TIMER

time_system::events_timer::events_timer() : timer(), events() {}

time_system::events_timer::events_timer(const float& time_) :timer(time_), events() {}

void time_system::events_timer::at_finish()
{
	this->events.call_events();
}
