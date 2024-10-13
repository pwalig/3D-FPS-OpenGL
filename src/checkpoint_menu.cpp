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

game::checkpoint_menu::checkpoint_menu() :
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

	this->served_player = game::player::players[0];
	this->setup_slots();
}

game::checkpoint_menu::~checkpoint_menu()
{
	for (ui_system::ui_draggable* uid : cubes) delete uid;
	time_system::time_scale = 1.0;
	glfwSetInputMode(engine::window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	if (game::checkpoint_menu::instance == this) game::checkpoint_menu::instance = nullptr;
}

game::checkpoint_menu::slot& game::checkpoint_menu::get_closest_slot(const glm::vec3& position)
{
	float min = std::numeric_limits<float>::max();
	slot min_slot = cube_slots[0];
	for (const slot& s : cube_slots) {
		if (glm::length(s.position - position) < min) {
			min = glm::length(s.position - position);
			min_slot = s;
		}
	}
	return min_slot;
}

void game::checkpoint_menu::setup_slots()
{
	cube_slots.push_back(slot(glm::vec3(0.5f, 0.3f, 0.1f)));
	float x = 0.1f;
	for (game::power_cube* c : served_player->hand_cubes) {
		ui_system::ui_draggable* uid = new ui_system::ui_draggable(glm::vec3(x, 0.5f, 0.01f), glm::vec2(0.1f, 0.1f), &(c->visual));
		uid->on_release.subscribe([uid, this]() {
			this->drop_cube(uid);
			});
		this->cubes.push_back(uid);
		this->cube_slots.push_back(slot(uid));
		x += 0.1f;
	}
	x = 0.6f;
	for (game::power_cube* c : served_player->gun_cubes) {
		ui_system::ui_draggable* uid = new ui_system::ui_draggable(glm::vec3(x, 0.5f, 0.01f), glm::vec2(0.1f, 0.1f), &(c->visual));
		uid->on_release.subscribe([uid, this]() {
			this->drop_cube(uid);
			});
		this->cubes.push_back(uid);
		this->cube_slots.push_back(slot(uid));
		x += 0.1f;
	}
}

void game::checkpoint_menu::pick_up_cube(const ui_system::ui_draggable* cube)
{
	for (slot& s : cube_slots) {
		if (s.occupant == cube) s.occupant = nullptr;
	}
}

void game::checkpoint_menu::drop_cube(ui_system::ui_draggable* cube)
{
	slot& s1 = this->game::checkpoint_menu::get_closest_slot(cube->position);
	if (s1.occupant) {
		for (slot& s : cube_slots) {
			if (s.occupant == cube) s.occupant->update_position(s.position);
		}
	}
	else {
		pick_up_cube(cube);
		s1.drop_cube(cube);
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
	position(position_), occupant(nullptr)
{
}

game::checkpoint_menu::slot::slot(ui_system::ui_draggable* draggable) :
	position(draggable->position), occupant(draggable)
{
}

void game::checkpoint_menu::slot::drop_cube(ui_system::ui_draggable* cube)
{
	if (this->occupant) throw std::logic_error("drop ad occupied slot attempted");
	this->occupant = cube;
	cube->update_position(this->position);
}
