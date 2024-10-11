#pragma once
#include "gate.h"

namespace game {
	class level_gate : public game::gate {
	public:
		level_gate(const glm::vec3& position, const glm::quat& rotation_, const glm::vec3& size, const std::vector<std::string>& scene1_, const std::vector<std::string>& scene2_);

		std::vector<std::string> scenes1; // vector of scenes to load / un load
		std::vector<std::string> scenes2; // vector of scenes to load / un load
	};
}
