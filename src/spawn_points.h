#pragma once
#include "game_script.h"
#include <glm/glm.hpp>
#include <vector>
namespace game {
	class spawn_point : public scripts_system::script {
	public:
		spawn_point(const glm::vec3& position);
		~spawn_point();
		glm::vec3 coords;
		static std::vector<spawn_point*> spawn_points;
	};
}