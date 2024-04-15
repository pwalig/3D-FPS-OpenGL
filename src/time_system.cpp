#include "time_system.h"

double time::delta_time = 0.0f;
double time::time_scale = 1.0f;

engine::event_subscribtion_list time::timer_calls;
