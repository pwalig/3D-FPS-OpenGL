#pragma once
#include <event_subscription_list.h>

namespace time_system {
	class timer {
	protected:
		virtual void at_finish() {};
	public:
		float time;
		bool pause;
		bool die_on_finish;

		timer();
		timer(const float& time_);
		void start(const float& time_);
		void update();
		void stop(); // terminates counting - time goes to 0, unsubscribed from updates, requires start to restart
		~timer();
	};

	class function_timer : public time_system::timer {
	protected:
		void at_finish() override;
	public:
		std::function<void()> function;

		function_timer(const std::function<void()>& function_ = []() {});
		function_timer(const float& time_, const std::function<void()>& function_ = []() {});
	};

	class events_timer : public time_system::timer {
	protected:
		void at_finish() override;
	public:
		engine::event_subscription_list<> events;

		events_timer();
		events_timer(const float& time_);
	};
 }