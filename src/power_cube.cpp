#include "power_cube.h"

void game::power_cube::use()
{
	if (this->t.time > 0.0f) return;
	this->on_use();
	t.start(this->cooldown);
}

game::power_cube::power_cube(game::player* owner) : t([owner]() {owner->update_active_cube(); }) // update active cube when this cube becomes ready
{}
