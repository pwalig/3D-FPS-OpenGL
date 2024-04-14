#pragma once
#include <functional>
#include <game_script.h>

#define ENGINE_AT_UPDATE 0
#define ENGINE_AT_START 1
#define ENGINE_AT_AMOUNT 2

namespace engine {
	extern double delta_time;
	extern double time_scale;

	void initialize();
	void free();

	void subscribe(void(*_event)(), const int& type = ENGINE_AT_UPDATE);
	void subscribe(std::function<void()> _event, const int& type = ENGINE_AT_UPDATE);
	void subscribe(script* script);

	void call_events(const int& type = ENGINE_AT_UPDATE);
}