#pragma once
#include "game_script.h"
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include "gate.h"

namespace game {
	class level_segment : public scripts_system::script {
	public:
		level_segment(const std::string& scene_file);
		level_segment(game::gate& entry_gate);
		level_segment(const game::level_segment& other) = delete;
		level_segment(game::level_segment&& other) = delete;
	private:
		static std::vector<std::string> scene_pool;
		static std::string name_from_coords(const glm::vec3& coords);
		static std::vector<std::string> random_from_pool(const std::vector<game::gate*>& gates);
		static std::string random_from_pool(const game::gate& gate);
		static std::vector<std::string> get_pool(const glm::quat& entry_rotation);
	};
}

