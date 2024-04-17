#pragma once
#include <event_subscription_list.h>

namespace time_system {
	extern double delta_time;
	extern double time_scale;

	extern engine::event_subscription_list<> timer_calls;
}