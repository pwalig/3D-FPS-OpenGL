#include "entity.h"
#include <scripts_system.h>

void game::entity::damage(float damage)
{
	this->hp -= damage;
}

void game::entity::die()
{
	scripts_system::safe_destroy(this);
}
