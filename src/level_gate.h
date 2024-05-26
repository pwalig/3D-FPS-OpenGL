#pragma once
#include "game_script.h"
#include "physics.h"

#define LEVEL_GATE_STATE_ON_SIDE_1 0
#define LEVEL_GATE_STATE_ONLY_1 1
#define LEVEL_GATE_STATE_BOTH 2
#define LEVEL_GATE_STATE_ONLY_2 3
#define LEVEL_GATE_STATE_ON_SIDE_2 4
#define LEVEL_GATE_STATE_UNKNOWN 5

namespace game {
	class level_gate : public scripts_system::script {
	public:
		level_gate(const glm::vec3& position, const glm::quat& rotation_, const glm::vec3& size, const std::vector<std::string>& scene1_, const std::vector<std::string>& scene2_);

		unsigned __int8 state = LEVEL_GATE_STATE_UNKNOWN;
		void on_pass();
	private:
		glm::vec3 pos1;
		glm::vec3 pos2;
		glm::quat rotation;
		physics::colliders::box col1;
		physics::colliders::box col2;

		std::vector<std::string> scenes1; // vector of scenes to load / un load
		std::vector<std::string> scenes2; // vector of scenes to load / un load
	};
}
