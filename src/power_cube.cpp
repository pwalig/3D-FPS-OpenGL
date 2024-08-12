#include "power_cube.h"
#include <glm/geometric.hpp>

void game::power_cube::use()
{
	if (this->t.time > 0.0f) return;
	this->on_use();
	t.start(this->cooldown);
}

game::power_cube::power_cube(game::player* owner) :
	t([owner]() {
		owner->update_active_cube(); // update active cube when this cube becomes ready
		}
	)
{
	visual.model_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
	visual_rb.mass = 1.0f;
	visual_rb.angular_speed = glm::vec3(90.0f);
}

void game::power_cube::update()
{
	visual_rb.force = (target_ui_pos - visual_rb.position) * 100.0f;
	visual_rb.movement_drag = 10.0f / (glm::length(target_ui_pos - visual_rb.position) + 0.5f);
	visual.anchor_point = visual_rb.position;
	visual.model_matrix = glm::scale(glm::toMat4(visual_rb.rotation), glm::vec3(0.07f));
}

void game::power_cube::set_ui_position(const glm::vec3 & new_position)
{
	target_ui_pos = new_position;
	visual.anchor_point = new_position;
	visual_rb.position = new_position;
}
