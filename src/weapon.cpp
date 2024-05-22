#include "weapon.h"
#include "gameplay_manager.h"

void game::weapon::hit_scan(const physics::ray& r, const std::function<void(game::entity*)>& on_hit, const std::function<void()>& on_miss)
{
	if (gameplay_manager::game_paused) return;
	physics::ray_intersection_info ri = physics::ray_cast(r);
	if (ri.intersect == RAY_INTERSECT_NONE) {
		on_miss();
		printf("no ray hit\n");
		return;
	}
	printf("ri: %s, %d, %f\n", ri.col->owner->name.c_str(), ri.intersect, ri.distance);
	if (game::entity* ent = dynamic_cast<game::entity*>(ri.col->owner)) {
		// we collided with entity
		on_hit(ent);

		printf("new hp = %d\n", ent->hp);
	}
	else {
		on_miss();
	}
}
