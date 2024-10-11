#include "level_gate.h"
#include "scripts_system.h"
#include <scene_loader.h>

game::level_gate::level_gate(const glm::vec3& position, const glm::quat& rotation_, const glm::vec3& size, const std::vector<std::string>& scene1_, const std::vector<std::string>& scene2_) :
	pos1(position + ((rotation_ * glm::vec3(0.0f, 0.5f, 0.0f)) * size.y)),
	pos2(position - ((rotation_ * glm::vec3(0.0f, 0.5f, 0.0f)) * size.y)),
	rotation(rotation_), col1(pos1, rotation, size), col2(pos2, rotation, size), scenes1(scene1_), scenes2(scene2_)
{
	col1.layer = COLLISION_LAYERS_LEVEL_GATES;
	col2.layer = COLLISION_LAYERS_LEVEL_GATES;

	col1.on_collision_enter.subscribe([this](physics::collision_info ci) {
#ifdef _DEBUG
		printf("%s c1 enter\n", this->name.c_str());
#endif
		switch (this->gate_state)
		{
		case state::unknown:
		case state::onSide1:
			this->gate_state = state::only1;
			break;
		case state::only2:
			this->gate_state = state::both;
			break;
		default:
#ifdef _DEBUG
			printf("%s wrong passthrough\n", this->name.c_str());
#endif
			break;
		}
#ifdef _DEBUG
		printf("%s state: %d\n", this->name.c_str(), this->gate_state);
#endif
		});

	col2.on_collision_enter.subscribe([this](physics::collision_info ci) {
#ifdef _DEBUG
		printf("%s c2 enter\n", this->name.c_str());
#endif
		switch (this->gate_state)
		{
		case state::unknown:
		case state::onSide2:
			this->gate_state = state::only2;
			break;
		case state::only1:
			this->gate_state = state::both;
			break;
		default:
#ifdef _DEBUG
			printf("%s wrong passthrough\n", this->name.c_str());
#endif
			break;
		}
#ifdef _DEBUG
		printf("%s state: %d\n", this->name.c_str(), this->gate_state);
#endif
		});

	col1.on_collision_exit.subscribe([this](physics::collider* other) {
#ifdef _DEBUG
		printf("%s c1 exit\n", this->name.c_str());
#endif
		switch (this->gate_state)
		{
		case state::only1:
			this->gate_state = state::onSide1;
			this->on_pass();
			break;
		case state::both:
			this->gate_state = state::only2;
			break;
		default:
#ifdef _DEBUG
			printf("%s wrong passthrough\n", this->name.c_str());
#endif
			break;
		}
#ifdef _DEBUG
		printf("%s state: %d\n", this->name.c_str(), this->gate_state);
#endif
		});

	col2.on_collision_exit.subscribe([this](physics::collider* other) {
#ifdef _DEBUG
		printf("%s c2 exit\n", this->name.c_str());
#endif
		switch (this->gate_state)
		{
		case state::only2:
			this->gate_state = state::onSide2;
			this->on_pass();
			break;
		case state::both:
			this->gate_state = state::only1;
			break;
		default:
#ifdef _DEBUG
			printf("%s wrong passthrough\n", this->name.c_str());
#endif
			break;
		}
#ifdef _DEBUG
		printf("%s state: %d\n", this->name.c_str(), this->gate_state);
#endif
		});
}


void game::level_gate::on_pass()
{
	if (this->gate_state == state::onSide1) { // we are on side 1
		for (std::string scene : scenes2) scripts_system::events[SCRIPTS_START].subscribe([scene]() {scene_loader::un_load_scene(scene); });
		for (std::string scene : scenes1) scripts_system::events[SCRIPTS_START].subscribe([scene]() {scene_loader::load_scene(scene); });
#ifdef _DEBUG
		printf("%s: side 1 entered\n", this->name.c_str());
#endif
	}
	else if (this->gate_state == state::onSide2) { // we are on side 2
		for (std::string scene : scenes1) scripts_system::events[SCRIPTS_START].subscribe([scene]() {scene_loader::un_load_scene(scene); });
		for (std::string scene : scenes2) scripts_system::events[SCRIPTS_START].subscribe([scene]() {scene_loader::load_scene(scene); });
#ifdef _DEBUG
		printf("%s: side 2 entered\n", this->name.c_str());
#endif
	}
#ifdef _DEBUG
	else printf("level_gate error: pass called on in between state\n");
#endif
}
