#include "time_system.h"

double time_system::delta_time = 0.0f;
double time_system::time_scale = 1.0f;

engine::event_subscribtion_list time_system::timer_calls;
