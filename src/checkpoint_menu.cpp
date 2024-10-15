#include "checkpoint_menu.h"
#include <glm/ext/matrix_transform.hpp>
#include <scripts_system.h>
#include "graphics_menu.h"
#include "pause_menu.h"
#include <gameplay_manager.h>
#include <time_system.h>
#include <engine.h>
#include <power_cube.h>
#include <stdexcept>

game::checkpoint_menu* game::checkpoint_menu::instance = nullptr;

const float game::checkpoint_menu::cube_depth = 0.1f;
const float game::checkpoint_menu::slot_depth = 0.2f;

game::checkpoint_menu::checkpoint_menu(game::player* player_to_serve) :
	served_player(player_to_serve),
	title("CHECKPOINT", "../assets/fonts/bitmap/handwiriting-readable.png",
		glm::vec3(0.5f, 0.95f, 0.2f), glm::scale(glm::mat4(1.0f), glm::vec3(0.03f, 0.05f, 1.0f))),
	text("drag cubes in right spots to customize your build", "../assets/fonts/bitmap/handwiriting-readable.png",
		glm::vec3(0.5f, 0.8f, 0.2f), glm::scale(glm::mat4(1.0f), glm::vec3(0.02f, 0.037f, 1.0f))),
	back(glm::vec3(0.5f, 0.1f, 0.2f), glm::vec2(0.07f, 0.03f), "../assets/textures/White_Square.png", "BACK", "../assets/fonts/bitmap/handwiriting-readable.png")
{
	// singleton stuff
	if (game::checkpoint_menu::instance) {
		printf("soft error: attempt to create second checkpoint menu\n");
		delete this;
		return;
	}
	game::checkpoint_menu::instance = this;

	// STYLE
	// back
	back.text.color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

	// FUNCTION
	this->back.on_click.subscribe([]() { scripts_system::safe_destroy(game::checkpoint_menu::instance); });

	this->setup_slots();
}

game::checkpoint_menu::~checkpoint_menu()
{
	for (cube& c : cubes) delete (c.draggable);
	apply_configuration();
	time_system::time_scale = 1.0;
	glfwSetInputMode(engine::window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	if (game::checkpoint_menu::instance == this) game::checkpoint_menu::instance = nullptr;
}

game::checkpoint_menu::slot* game::checkpoint_menu::get_closest_slot(const glm::vec3& position)
{
	float min = std::numeric_limits<float>::max();
	slot* min_slot = &cube_slots[0];
	for (slot& s : cube_slots) {
		if (glm::length(s.position - position) < min) {
			min = glm::length(s.position - position);
			min_slot = &s;
		}
	}
	return min_slot;
}

void game::checkpoint_menu::setup_slots()
{
	const float cubes_height = 0.7f;
	cubes.reserve(served_player->max_gun_cubes + served_player->max_hand_cubes + served_player->unused_cubes.size());
	cube_slots.reserve((served_player->max_gun_cubes + served_player->max_hand_cubes) * 2 + served_player->unused_cubes.size());
	float x = 0.1f;
	for (int i = 0; i < served_player->max_hand_cubes; ++i) {
		if (i < served_player->hand_cubes.size())
			add_cube_and_slot(served_player->hand_cubes[i], glm::vec3(x, cubes_height, cube_depth), slot_type::Hand);
		else cube_slots.push_back(slot(glm::vec3(x, cubes_height, slot_depth), slot_type::Hand)); // free slot
		x += 0.1f;
	}
	x = 0.6f;
	for (int i = 0; i < served_player->max_gun_cubes; ++i) {
		if (i < served_player->gun_cubes.size())
			add_cube_and_slot(served_player->gun_cubes[i], glm::vec3(x, cubes_height, cube_depth), slot_type::Gun);
		else cube_slots.push_back(slot(glm::vec3(x, cubes_height, slot_depth), slot_type::Gun)); // free slot
		x += 0.1f;
	}
	float y = 0.5f;
	x = 0.1f;
	for (const cube& c : this->cubes) {
		cube_slots.push_back(slot(glm::vec3(x, y, slot_depth))); // free slot
		x += 0.1f;
		if (x > 0.95f) {
			x = 0.1f;
			y -= 0.1f;
		}
	}
	for (game::power_cube* c : served_player->unused_cubes) {
		add_cube_and_slot(c, glm::vec3(x, y, cube_depth), slot_type::Empty);
		x += 0.1f;
		if (x > 0.95f) {
			x = 0.1f;
			y -= 0.1f;
		}
	}
}

void game::checkpoint_menu::add_cube_and_slot(game::power_cube* cube_, const glm::vec3& position, const slot_type& type_)
{
	this->cubes.push_back(cube());
	cube* cp = &(this->cubes.back());
	cp->ptr = cube_;
	cp->draggable = new ui_system::ui_draggable(position, glm::vec2(0.1f, 0.1f), &(cube_->visual));
	cp->draggable->on_release.subscribe([cp, this]() {
		this->drop_cube(cp);
		});
	this->cube_slots.push_back(slot(cp, type_));
	cp->slot_ptr = &(this->cube_slots.back());
	
}

void game::checkpoint_menu::pick_up_cube(cube* cube_)
{
	for (slot& s : cube_slots) {
		if (s.occupant == cube_) s.occupant = nullptr;
	}
}

void game::checkpoint_menu::drop_cube(cube* cube_)
{
	slot* s1 = this->game::checkpoint_menu::get_closest_slot(cube_->draggable->position);
	if (s1->occupant) {
		for (slot& s : cube_slots) {
			if (s.occupant == cube_) s.occupant->draggable->update_position(s.position);
		}
	}
	else {
		pick_up_cube(cube_);
		s1->drop_cube(cube_);
	}
}

game::checkpoint_menu::slot& game::checkpoint_menu::get_empty_slot()
{
	for (slot& s : cube_slots) {
		if (s.occupant == nullptr) return s;
	}
	throw std::logic_error("no empty slots");
}

void game::checkpoint_menu::apply_configuration()
{
	served_player->gun_cubes.clear();
	served_player->hand_cubes.clear();
	served_player->unused_cubes.clear();
	for (const slot& s : this->cube_slots) {
		if (s.occupant == nullptr) continue;
		if (s.type == slot_type::Hand)
			served_player->hand_cubes.push_back(s.occupant->ptr);
		else if (s.type == slot_type::Gun) 
			served_player->gun_cubes.push_back(s.occupant->ptr);
		else if (s.type == slot_type::Empty)
			served_player->unused_cubes.push_back(s.occupant->ptr);
	}
	served_player->update_active_gun();
	served_player->update_active_cube();
	served_player->update_ui_cube_colors();
	served_player->set_ui_cube_positions();
}

game::checkpoint_menu::slot::slot(const glm::vec3& position_, const slot_type& type_) :
	position(position_), occupant(nullptr), type(type_),
	image(
		"../assets/UI/crosshair.png",
		glm::vec3(position_.x, position_.y, slot_depth),
		glm::scale(glm::mat4(1.0f), glm::vec3(0.01f, 0.01f, 1.0f))
	)
{
}

game::checkpoint_menu::slot::slot(cube* cube_, const slot_type& type_) :
	position(cube_->draggable->position), occupant(cube_), type(type_),
	image(
		"../assets/UI/crosshair.png",
		glm::vec3(cube_->draggable->position.x, cube_->draggable->position.y, slot_depth),
		glm::scale(glm::mat4(1.0f), glm::vec3(0.01f, 0.01f, 1.0f))
	)
{
}

void game::checkpoint_menu::slot::drop_cube(cube* cube_)
{
	if (this->occupant) throw std::logic_error("drop ad occupied slot attempted");
	this->occupant = cube_;
	cube_->draggable->update_position(this->position);
}
