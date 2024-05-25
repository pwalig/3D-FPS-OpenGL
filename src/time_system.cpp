#include "time_system.h"


double time_system::delta_time = 0.0f;
double time_system::time_scale = 1.0f;

engine::object_subscription_list<time_system::timer> time_system::timers;

void time_system::call_in(const std::function<void()>& function, const float& time)
{
	time_system::function_timer* ft = new function_timer(time, function);
	ft->die_on_finish = true;
	time_system::timers.subscribe(ft);
}
