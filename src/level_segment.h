#pragma once
#include "game_script.h"
#include <vector>
#include <string>

namespace game {
	class level_segment : public scripts_system::script {
	public:
		level_segment(const std::string& scene, const std::vector<std::string>& scene_pool);
		level_segment(const game::level_segment& other) = delete;
		level_segment(game::level_segment&& other) = delete;
	};
}

