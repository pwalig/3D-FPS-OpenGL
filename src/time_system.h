#pragma once
#include <event_subscription_list.h>
#include <object_subscription_list.h>
#include <timer.h>

namespace time_system {
	extern double delta_time;
	extern double time_scale;

	extern engine::object_subscription_list<time_system::timer> timers;

	void call_in(const std::function<void()>& function, const float& time);
}