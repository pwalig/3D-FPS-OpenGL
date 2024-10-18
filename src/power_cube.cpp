#include "power_cube.h"
#include <glm/geometric.hpp>
#include <time_system.h>

void game::power_cube::use()
{
	if (this->t.time > 0.0f) return;
	this->preset->on_use(this->owner);
	t.start(this->preset->cooldown);
	ui_cooldown.play(this->preset->cooldown / 36.0f);
}

void game::power_cube::heal()
{
	owner->heal(preset->healing);
	t.start(this->preset->cooldown);
	ui_cooldown.play(this->preset->cooldown / 36.0f);
}

game::power_cube::power_cube(game::player* owner_, game::cube_preset* preset_) :
	t([owner_]() {
		owner_->update_active_cube(); // update active cube when this cube becomes ready
		}
	), owner(owner_), preset(preset_),
	ui_cooldown("../assets/UI/cube_cooldown.png", 6, 6)
{
	visual.model_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
	visual.color = preset->color;
	visual_rb.mass = 1.0f;
	visual_rb.angular_speed = glm::vec3(90.0f);
	ui_cooldown.model_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.07f));
	ui_cooldown.loop = false;
}

void game::power_cube::update()
{
	if (time_system::time_scale != 0.0) {
		visual_rb.force = (target_ui_pos - visual_rb.position) * 100.0f;
		visual_rb.movement_drag = 10.0f / (glm::length(target_ui_pos - visual_rb.position) + 0.5f);
		visual.anchor_point = visual_rb.position;
		visual_rb.rotation *= glm::quat(glm::vec3(-input_system::mouse_delta.y * owner->rot_speed, -input_system::mouse_delta.x * owner->rot_speed, 0.0f) * (float)time_system::time_scale);
		visual.model_matrix = glm::scale(glm::toMat4(visual_rb.rotation), glm::vec3(0.07f));
		ui_cooldown.anchor_point = visual_rb.position;
		ui_cooldown.anchor_point.z += 0.2f + (visual_rb.position.x / 100.0f);
		ui_cooldown.color.a = 1.0f / (glm::length(visual_rb.velocity) + 1.0f);
	}
}

void game::power_cube::set_ui_position(const glm::vec3 & new_position)
{
	target_ui_pos = new_position;
	visual.anchor_point = new_position;
	visual_rb.position = new_position;
}

game::cube_preset game::cube_presets::jumping = {
	3.0f, // cooldown
	25, // healing
	[](game::player* owner) {
	printf("jump_cube\n");
		float jmp = owner->jump_force; // store previous jump force
		owner->jump_force = 20.0f; // increase jump force
		time_system::call_in([owner, jmp]() { owner->jump_force = jmp; }, 3.0f); // set jump force back to normal after 3 seconds
		}, // on use
	glm::vec4(0.0f, 0.0f, 1.0f, 1.0f) // color
};

game::cube_preset game::cube_presets::speed = {
	3.0f, // cooldown
	25, // healing
	[](game::player* owner) {
		printf("speed_cube\n");
		float spd = owner->max_speed; // store previous speed
		owner->max_speed = 13.0f; // increase speed
		time_system::call_in([owner, spd]() { owner->max_speed = spd; }, 5.0f); // set speed back to normal after 3 seconds
		}, // on use
	glm::vec4(1.0f, 1.0f, 0.0f, 1.0f) // color
};

game::cube_preset game::cube_presets::dash = {
	3.0f, // cooldown
	25, // healing
	[](game::player* owner) {
		printf("dash_cube\n");
		owner->use_dash(36.0f, 0.11f, 0.5f);
		}, // on use
	glm::vec4(1.0f, 0.0f, 1.0f, 1.0f) // color
};

game::cube_preset game::cube_presets::missle = {
	3.0f, // cooldown
	25, // healing
	[](game::player* owner) {
		printf("missle_cube\n");
		owner->use_weapon(game::weapons::throwable_cube);
		}, // on use
	glm::vec4(1.0f, 0.0f, 0.0f, 1.0f) // color
};

game::collectable_cube::collectable_cube(const glm::vec3& position_, const float& radius) :
	visual(), position(position_), col(position, radius), l(position_)
{
	game::cube_preset* cp = game::cube_preset::get_random();
	visual.model_matrix = glm::scale(glm::translate(glm::mat4(1.0f), position), glm::vec3(1.0f));
	visual.albedo_ = cp->color;
	l.color = cp->color;
}

std::vector<game::cube_preset*> game::cube_preset::all = {
	&game::cube_presets::jumping,
	&game::cube_presets::speed,
	&game::cube_presets::dash,
	&game::cube_presets::missle
};

game::cube_preset* game::cube_preset::get_random()
{
	return all[rand() % all.size()];
}
