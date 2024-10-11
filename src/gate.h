#pragma once
#include "game_script.h"
#include "physics.h"
#include <functional>

namespace game {
	class gate : public scripts_system::script {
	public:
		enum class state { onSide1, only1, both, only2, onSide2, unknown };
		gate(
			const glm::vec3& position,
			const glm::quat& rotation_,
			const glm::vec3& size,
			const std::function<void()>& on_pass1_ = []() {},
			const std::function<void()>& on_pass2_ = []() {}
		);

		std::function<void()> on_pass1;
		std::function<void()> on_pass2;
		glm::vec3 get_position() const;
		glm::quat get_rotation() const;
	private:
		state gate_state = state::unknown;
		glm::vec3 pos1;
		glm::vec3 pos2;
		glm::quat rotation;
		physics::colliders::box col1;
		physics::colliders::box col2;
	};
}
