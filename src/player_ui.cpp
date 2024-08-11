#include "player_ui.h"
#include <glm/ext/matrix_transform.hpp>
#include <time_system.h>

game::player_ui::player_ui() :
	crosshair("../assets/UI/crosshair.png", glm::vec3(0.5f, 0.5f, 0.5f), glm::scale(glm::mat4(1.0f), glm::vec3(0.006f))),
	hp_bar("../assets/UI/red.png", glm::vec3(0.0f, 0.95f, 0.5f), glm::scale(glm::mat4(1.0f), glm::vec3(0.1f, 0.01f, 1.0f)), glm::vec3(-1.0f, 0.0f, 0.0f))
{
	crosshair.color.a = 0.7f;
	hp_bar.color.a = 0.7f;
}

void game::player_ui::update()
{
	for (ui_system::ui_model& pc : power_cubes) {
		pc.model_matrix = glm::rotate(pc.model_matrix, (float)time_system::delta_time, glm::vec3(1.0f));
	}
}
