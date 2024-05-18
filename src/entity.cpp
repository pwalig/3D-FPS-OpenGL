#include "entity.h"

void game::entity::damage(float damage)
{
	this->hp -= damage;
}
