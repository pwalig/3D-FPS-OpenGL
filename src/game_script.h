#pragma once
#include <string>
namespace scripts_system {
	class script {
	public:
		std::string name;

		script() : name("") {};
		script(const std::string& name_) : name(name_) {};

		virtual void init() {};
		virtual void start() {};
		virtual void update() {};
		virtual void free() {};

		virtual ~script() {};
	};
}