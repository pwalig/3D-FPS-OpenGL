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

		bool _running = false;
		std::vector<record> _events;
		std::vector<int*> _deletions;

		void _unsubscribe(const int& id);

	public:
		int* subscribe(const std::function<void(Args...)>& event_); // returns index

		void unsubscribe(int* id); // takes index returned by subscribe

		void call_events(Args... a);

		void clear();
	};

	template<typename ...Args>
	inline event_subscription_list<Args...>::record::record(const std::function<void(Args...)>& event_, const int& id_) : _event(event_), id(id_) {}
}

template<typename ...Args>
inline void engine::event_subscription_list<Args...>::_unsubscribe(const int& id)
{
	if (id < 0 || id >= this->_events.size()) { // wrong index check
		printf("unsubscription error: %d\n", id);
		return;
	}
	this->_events.erase(this->_events.begin() + id);
	if (_events.size() > 0) {
		for (auto it = this->_events.begin() + id; it != this->_events.end(); ++it) {
			--(it->id);
		}
	}
}

template<typename ...Args>
inline int* engine::event_subscription_list<Args...>::subscribe(const std::function<void(Args...)>& event_)
{
	int id = _events.size();
	this->_events.push_back(record(event_, id));
	return &(this->_events.back().id);
}

template<typename ...Args>
inline void engine::event_subscription_list<Args...>::unsubscribe(int* id)
{
	if (this->_running) this->_deletions.push_back(id); // events running -> schedule unsubscription
	else this->_unsubscribe(*id); // events not running -> unsubscribe now
}

template<typename ...Args>
inline void engine::event_subscription_list<Args...>::call_events(Args... args)
{
	this->_running = true; // event calling started

	auto end = this->_events.end();

	int siz = this->_events.size();
	for (auto it = this->_events.begin(); it != this->_events.begin() + siz; ++it) {
		const auto d = std::distance(_events.begin(), it);
		it->_event(args...);
		it = _events.begin();
		std::advance(it, d);
	}

	// perform scheduled deletions
	for (int* id : _deletions) {
		this->_unsubscribe(*id);
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
