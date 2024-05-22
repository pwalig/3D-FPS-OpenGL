#include "entity.h"
#include <scripts_system.h>

void game::entity::damage(int damage)
{
	this->hp -= damage;
	if (this->hp <= 0) this->die();
}

void game::entity::die()
{
	scripts_system::safe_destroy(this);
}
