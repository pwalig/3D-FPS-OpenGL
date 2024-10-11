#include "level_gate.h"
#include "scripts_system.h"
#include <scene_loader.h>

game::level_gate::level_gate(
	const glm::vec3& position,
	const glm::quat& rotation_,
	const glm::vec3& size,
	const std::vector<std::string>& scene1_,
	const std::vector<std::string>& scene2_
) :
	gate(position, rotation_, size,
		[this]() {
			for (std::string scene : this->scenes2) scripts_system::events[SCRIPTS_START].subscribe([scene]() {scene_loader::un_load_scene(scene); });
			for (std::string scene : this->scenes1) scripts_system::events[SCRIPTS_START].subscribe([scene]() {scene_loader::load_scene(scene); });
#ifdef _DEBUG
			printf("%s: side 1 entered\n", this->name.c_str());
#endif
		},
		[this]() { // we are on side 2
			for (std::string scene : this->scenes1) scripts_system::events[SCRIPTS_START].subscribe([scene]() {scene_loader::un_load_scene(scene); });
			for (std::string scene : this->scenes2) scripts_system::events[SCRIPTS_START].subscribe([scene]() {scene_loader::load_scene(scene); });
#ifdef _DEBUG
			printf("%s: side 2 entered\n", this->name.c_str());
#endif
		}
	), scenes1(scene1_), scenes2(scene2_)
{}
