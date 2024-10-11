#pragma once
#include "game_script.h"
#include "physics.h"

namespace game {
	class level_gate : public scripts_system::script {
	private:
		enum class state{ onSide1, only1, both, only2, onSide2, unknown };
		state gate_state = state::unknown;
	public:
		level_gate(const glm::vec3& position, const glm::quat& rotation_, const glm::vec3& size, const std::vector<std::string>& scene1_, const std::vector<std::string>& scene2_);

		void on_pass();
		std::vector<std::string> scenes1; // vector of scenes to load / un load
		std::vector<std::string> scenes2; // vector of scenes to load / un load
	private:
		glm::vec3 pos1;
		glm::vec3 pos2;
		glm::quat rotation;
		physics::colliders::box col1;
		physics::colliders::box col2;
	};
}
