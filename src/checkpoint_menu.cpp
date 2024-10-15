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

game::checkpoint_menu::checkpoint_menu(game::player* player_to_serve) :
	served_player(player_to_serve),
	title("CHECKPOINT", "../assets/fonts/bitmap/handwiriting-readable.png",
		glm::vec3(0.5f, 0.95f, 0.2f), glm::scale(glm::mat4(1.0f), glm::vec3(0.03f, 0.05f, 1.0f))),
	text("checkpoints are under construction...\nthey do nothing for now...", "../assets/fonts/bitmap/handwiriting-readable.png",
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
	float x = 0.1f;
	for (game::power_cube* c : served_player->hand_cubes) {
		add_cube_and_slot(c, glm::vec3(x, 0.5f, 0.01f));
		cube_slots.push_back(slot(glm::vec3(x, 0.3f, 0.1f))); // free slot
		x += 0.1f;
	}
	x = 0.6f;
	for (game::power_cube* c : served_player->gun_cubes) {
		add_cube_and_slot(c, glm::vec3(x, 0.5f, 0.01f));
		cube_slots.push_back(slot(glm::vec3(x, 0.3f, 0.1f))); // free slot
		x += 0.1f;
	}
}

void game::checkpoint_menu::add_cube_and_slot(game::power_cube* cube, const glm::vec3& position)
{
	game::checkpoint_menu::cube cu;
	cu.draggable = new ui_system::ui_draggable(position, glm::vec2(0.1f, 0.1f), &(cube->visual));
	cu.draggable->on_release.subscribe([cu, this]() {
		this->drop_cube(cu.draggable);
		});
	this->cube_slots.push_back(slot(cu.draggable));
	this->cubes.push_back(cu);
}

void game::checkpoint_menu::pick_up_cube(const ui_system::ui_draggable* cube)
{
	for (slot& s : cube_slots) {
		if (s.occupant == cube) s.occupant = nullptr;
	}
}

void game::checkpoint_menu::drop_cube(ui_system::ui_draggable* cube)
{
	slot* s1 = this->game::checkpoint_menu::get_closest_slot(cube->position);
	if (s1->occupant) {
		for (slot& s : cube_slots) {
			if (s.occupant == cube) s.occupant->update_position(s.position);
		}
	}
	else {
		pick_up_cube(cube);
		s1->drop_cube(cube);
	}
}

game::checkpoint_menu::slot& game::checkpoint_menu::get_empty_slot()
{
	for (slot& s : cube_slots) {
		if (s.occupant == nullptr) return s;
	}
	throw std::logic_error("no empty slots");
}

game::checkpoint_menu::slot::slot(const glm::vec3& position_) :
	position(position_), occupant(nullptr),
	image("../assets/UI/crosshair.png", position_ + glm::vec3(0.0f, 0.0f, 0.2f), glm::scale(glm::mat4(1.0f), glm::vec3(0.01f, 0.01f, 1.0f)))
{
}

game::checkpoint_menu::slot::slot(ui_system::ui_draggable* draggable) :
	position(draggable->position), occupant(draggable),
	image("../assets/UI/crosshair.png", draggable->position + glm::vec3(0.0f, 0.0f, 0.2f), glm::scale(glm::mat4(1.0f), glm::vec3(0.01f, 0.01f, 1.0f)))
{
}

void game::checkpoint_menu::slot::drop_cube(ui_system::ui_draggable* cube)
{
	if (this->occupant) throw std::logic_error("drop ad occupied slot attempted");
	this->occupant = cube;
	cube->update_position(this->position);
}
