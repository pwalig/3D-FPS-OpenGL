#pragma once
#include <string>
namespace scripts_system {
	class script {
	public:
		std::string name;

		script(const std::string& name_ = "script");

		virtual void start() {};
		virtual void update() {};

		virtual ~script();
	};
}