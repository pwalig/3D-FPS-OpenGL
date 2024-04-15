#pragma once
#include <event_subscribtion_list.h>
namespace time {
	class timer {
	private:
		int _id;
		bool _paused;
		void update();
	public:
		float time;
		engine::event_subscribtion_list events;

		timer();
		void start(const float& time_);
		void pause();
		void resume();
		void stop(); // terminates counting - time goes to 0, unsubscribed from updates, requires start to restart
		~timer();
	};
 }