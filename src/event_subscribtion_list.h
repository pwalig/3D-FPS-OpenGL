#pragma once
#include <vector>
#include <functional>

namespace engine {
	class event_subscribtion_list {
	private:
		std::vector<void(*)()> _events;
		std::vector<std::function<void()>> _events_f;
	public:
		int subscribe(void(*_event)()); // returns index
		int subscribe(const std::function<void()>& _event); // returns index
		//void unsubscribe(void(*_event)()); // not working
		void unsubscribe(void(*_event)(), const int& id);
		void unsubscribe(const std::function<void()>& _event, const int& id);

		void unsubscribe(const int& id);
		void unsubscribe_f(const int& id);

		void call_events() const;
	};
}