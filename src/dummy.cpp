#include "dummy.h"
#include <time_system.h>

void game::dummy::damage(int damage, glm::vec3 damage_source_position)
{
	damage_sum += damage;
	printf("damage: %d, total: %d, dps: %f\n", damage, damage_sum, (float)damage_sum / time);
}

game::dummy::dummy() : po()
{
	po.name = this->name + "_po";
	po.col.layer = COLLISION_LAYERS_ENEMIES;
	po.col.owner = this;
	po.col.size = glm::vec3(2.0f);
	po.rb.dynamic = false;
	po.rb.position = glm::vec3(0.0f, 3.0f, 0.0f);
}

void game::dummy::update()
{
	if (damage_sum > 0) this->time += (float)time_system::delta_time;
}
