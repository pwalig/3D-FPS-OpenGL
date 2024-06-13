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
			unsigned int id;
			record(const std::function<void(Args...)>& event_, const int& id_);
		};

		volatile bool _running = false; // prevent code optimisation in call_events (volatile)
		std::vector<record> _events;
		std::vector<unsigned int> _deletions;
		unsigned int id_gen = 0;

		void _unsubscribe(const unsigned int& id);

	public:
		unsigned int subscribe(const std::function<void(Args...)>& event_); // returns index

		void unsubscribe(const unsigned int& id); // takes index returned by subscribe

		void call_events(Args... a);

		void clear();
	};

	template<typename ...Args>
	inline event_subscription_list<Args...>::record::record(const std::function<void(Args...)>& event_, const int& id_) : _event(event_), id(id_) {}
}

template<typename ...Args>
inline void engine::event_subscription_list<Args...>::_unsubscribe(const unsigned int& id)
{
	for (int i = 0; i < _events.size(); ++i) {
		if (_events[i].id == id) {
			_events.erase(this->_events.begin() + i);
			break;
		}
	}
}

template<typename ...Args>
inline unsigned int engine::event_subscription_list<Args...>::subscribe(const std::function<void(Args...)>& event_)
{
	this->_events.push_back(record(event_, (this->id_gen)++));
	return this->_events.back().id;
}

template<typename ...Args>
inline void engine::event_subscription_list<Args...>::unsubscribe(const unsigned int& id)
{
	if (this->_running) this->_deletions.push_back(id);  // events running -> schedule unsubscription
	else this->_unsubscribe(id);  // events not running -> unsubscribe now
}

template<typename ...Args>
inline void engine::event_subscription_list<Args...>::call_events(Args... args)
{
	this->_running = true; // event calling started

	int siz = this->_events.size();
	for (auto it = this->_events.begin(); it != this->_events.begin() + siz; ++it) {
		const auto d = std::distance(_events.begin(), it);
		it->_event(args...);
		it = _events.begin();
		std::advance(it, d);
	}

	// perform scheduled deletions
	for (const unsigned int& id : _deletions) {
		this->_unsubscribe(id);
	}
	this->_deletions.clear();

	this->_running = false; // event calling ended
}

template<typename ...Args>
inline void engine::event_subscription_list<Args...>::clear()
{
	this->_events.clear();
	this->_deletions.clear();
}
