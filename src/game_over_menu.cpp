#include "game_over_menu.h"
#include <scene_loader.h>
#include <time_system.h>

game::game_over_menu::game_over_menu() : uit("game over [enter to retry]", "../assets/fonts/bitmap/handwiriting-readable.png", glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(860.0f, 540.0f, -10.0f)), glm::vec3(20.0f, 30.0f, 1.0f)))
{
	time_system::time_scale = 0.0f;
}

void game::game_over_menu::restart()
{
	scene_loader::free();
	scene_loader::load_scene("initial_scene.json");
	time_system::time_scale = 1.0f;
	scripts_system::safe_destroy(this);
}
