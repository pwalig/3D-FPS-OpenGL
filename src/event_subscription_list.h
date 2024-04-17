#pragma once
#include <vector>
#include <functional>

namespace engine {
	template <typename ...Args>
	class event_subscription_list {
	private:
		struct record {
		public:
			std::function<void(Args...)> _event;
			int id;
			record(const std::function<void(Args...)>& event_, const int& id_);
		};

		std::vector<record> _events;
	public:
		int* subscribe(const std::function<void(Args...)>& event_); // returns index

		void unsubscribe(int id); // takes index returned by subscribe

		void call_events(Args... a) const;
	};

	template<typename ...Args>
	inline event_subscription_list<Args...>::record::record(const std::function<void(Args...)>& event_, const int& id_) : _event(event_), id(id_) {}
}

template<typename ...Args>
inline int* engine::event_subscription_list<Args...>::subscribe(const std::function<void(Args...)>& event_)
{
	int id = _events.size();
	this->_events.push_back(record(event_, id));
	return &(this->_events.back().id);
}

template<typename ...Args>
inline void engine::event_subscription_list<Args...>::unsubscribe(int id)
{
	this->_events.erase(this->_events.begin() + id);
	if (_events.size() > 0) {
		for (auto it = this->_events.begin() + id; it != this->_events.end(); ++it) {
			--(it->id);
		}
	}
}

template<typename ...Args>
inline void engine::event_subscription_list<Args...>::call_events(Args... args) const
{
	for (auto it = this->_events.begin(); it != this->_events.end(); it < this->_events.end() ? ++it : it += 0) {
		const auto d = std::distance(_events.begin(), it);
		it->_event(args...);
		it = _events.begin();
		std::advance(it, d);
	}
}