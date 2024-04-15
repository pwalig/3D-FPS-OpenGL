#pragma once
namespace scripts_system {
	class script {
	public:
		bool active = true;
		virtual void init() {};
		virtual void start() {};
		virtual void update() {};
		virtual void free() {};
	};
}