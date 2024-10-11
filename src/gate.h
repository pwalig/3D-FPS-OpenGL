#pragma once
#include "game_script.h"
#include "physics.h"
namespace game {
	class gate : public scripts_system::script {
	public:
		enum class state { onSide1, only1, both, only2, onSide2, unknown };
		gate(
			const glm::vec3& position,
			const glm::quat& rotation_,
			const glm::vec3& size,
			const std::function<void(game::gate::state)>& on_pass = [](game::gate::state) {}
		);

		std::function<void(game::gate::state)> on_pass;
		glm::vec3 get_position() const;
	private:
		state gate_state = state::unknown;
		glm::vec3 pos1;
		glm::vec3 pos2;
		glm::quat rotation;
		physics::colliders::box col1;
		physics::colliders::box col2;
	};
}
