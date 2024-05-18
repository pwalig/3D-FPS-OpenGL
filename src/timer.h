#pragma once
#include <event_subscription_list.h>
namespace time_system {
	class timer {
	private:
		bool _paused;
	public:
		float time;
		engine::event_subscription_list<> events;

		timer();
		void start(const float& time_);
		void pause();
		void update();
		void resume();
		void stop(); // terminates counting - time goes to 0, unsubscribed from updates, requires start to restart
		~timer();
	};
 }