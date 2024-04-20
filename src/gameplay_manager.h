#pragma once
#include "scripts_system.h"
namespace game {
	class gameplay_manager : public scripts_system::script {
	public:
		gameplay_manager();
	private:
		bool game_paused;
		double _time_scale_buffor;
		void pause_un_pause();
		void pause();
		void un_pause();
	};
}