#include "segment_gate.h"

game::segment_gate::segment_gate(
	const glm::vec3& position, const glm::quat& rotation_, const glm::vec3& size
) : gate(
	position, rotation_, size,
	[this]() {this->on_pass1_clear(); this->on_pass1_spawn(); },
	[this]() {this->on_pass2_clear(); this->on_pass2_spawn(); }
	)
{}
