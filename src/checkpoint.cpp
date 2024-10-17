#include "checkpoint.h"
#include "input_system.h"
#include "time_system.h"
#include "checkpoint_menu.h"
#include <engine.h>

game::checkpoint::checkpoint(const glm::vec3& position_, const float& radius) :
	position(position_), col(position, radius), kb(nullptr), txt(nullptr),
	li(position_, glm::vec3(10.0f, 7.0f, 6.0f))
{
	col.layer = COLLISION_LAYERS_LEVEL_GATES;
	col.on_collision_enter.subscribe([this](physics::collision_info ci) {
		if (ci.other->owner->name == "player") {
			this->kb = new input_system::key_bind(std::bind(&game::checkpoint::toggle_menu, this), GLFW_KEY_Z, GLFW_PRESS);
			this->txt = new ui_system::ui_text(
				"USE CHECKPOINT (Z)", "../assets/fonts/bitmap/handwiriting-readable.png",
				glm::vec3(0.5f, 0.2f, 0.2f), glm::scale(glm::mat4(1.0f), glm::vec3(0.02f, 0.035f, 1.0f))
			);
		}
		});

	col.on_collision_exit.subscribe([this](physics::collider* c) {
		if (c->layer == COLLISION_LAYERS_PLAYER) {
			delete (this->kb);
			this->kb = nullptr;
			delete (this->txt);
			this->txt = nullptr;
		}
		});
}

void game::checkpoint::toggle_menu()
{
	if (game::checkpoint_menu::instance) {
		delete game::checkpoint_menu::instance;
	}
	else {
		time_system::time_scale = 0.0;
		glfwSetInputMode(engine::window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		new game::checkpoint_menu(game::player::get_closest_player(this->position));
	}
}
