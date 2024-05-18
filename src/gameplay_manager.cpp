#include "gameplay_manager.h"
#include <time_system.h>
#include <input_system.h>
#include <scene_loader.h>
#include "player_script.h"
#include "enemy.h"

glm::vec3* game::gameplay_manager::player_position = nullptr;

game::gameplay_manager::gameplay_manager() : _time_scale_buffor(1.0f), game_paused(false)
{
	this->_time_scale_buffor = time_system::time_scale;
}

void game::gameplay_manager::start()
{
	game::enemy* ene = scripts_system::instantiate<game::enemy, float>(5.0f, this, "enemy");
}

void game::gameplay_manager::pause_un_pause()
{
	if (this->game_paused) this->un_pause();
	else this->pause();
}

void game::gameplay_manager::pause()
{
	this->_time_scale_buffor = time_system::time_scale;
	time_system::time_scale = 0.0f;
	this->game_paused = true;
}

void game::gameplay_manager::un_pause()
{
	time_system::time_scale = this->_time_scale_buffor;
	this->game_paused = false;
}
