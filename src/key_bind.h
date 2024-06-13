#pragma once
#include <functional>

namespace input_system {
	class key_bind {
	public:
		key_bind(const std::function<void()>& function, const int& key, const int& action);
		~key_bind();
	private:
		int _action;
		int _key;
		unsigned int _id;
	};
}