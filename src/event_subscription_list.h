#pragma once
#include <vector>
#include <functional>

namespace engine {
	struct record {
	public:
		std::function<void()> _event;
		int* id;
		record(const std::function<void()>& event_, const int& id);
		record(const record& other);
		record& operator=(const record& other);
		record(record&& other) noexcept;
		record& operator=(record&& other) noexcept;
		~record();
	};

	class event_subscribtion_list {
	private:
		std::vector<record> _events;
	public:
		int* subscribe(const std::function<void()>& event_); // returns index

		void unsubscribe(int id); // takes index returned by subscribe

		void call_events() const;
	};
}