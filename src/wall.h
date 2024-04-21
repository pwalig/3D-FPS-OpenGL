#pragma once
#include "scripts_system.h"
#include <glm/glm.hpp>
#include <transform.h>
#include "physics.h"

namespace game {
	class wall : public scripts_system::script {
	public:
		wall(const glm::vec3& size);
		void update() override;
	private:
		physics::rigidbody rb;
		physics::colliders::plane col;
	};
}