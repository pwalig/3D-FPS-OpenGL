#include "weapon.h"
#include "gameplay_manager.h"
#include "hit_scan_damage_gun.h"
#include "projectile_damage_gun.h"


std::map<std::string, game::weapon*> game::weapon::weapon_map;
game::weapon* game::weapons::throwable_cube = nullptr;

void game::weapon::hit_scan(const physics::ray& r, const std::function<void(game::entity*)>& on_hit, const std::function<void()>& on_miss)
{
	if (gameplay_manager::game_paused) return;
	physics::ray_intersection_info ri = physics::ray_cast(r);
	if (ri.intersect == RAY_INTERSECT_NONE) {
		on_miss();
		printf("no ray hit\n");
		return;
	}
	if (ri.col->owner) printf("ri: %s, %d, %f\n", ri.col->owner->name.c_str(), ri.intersect, ri.distance);
	if (game::entity* ent = dynamic_cast<game::entity*>(ri.col->owner)) {
		// we collided with entity
		on_hit(ent);

		printf("new hp = %d\n", ent->hp);
	}
	else {
		on_miss();
	}
}

void game::weapon::init()
{
	weapon_map["ab"] = new game::hit_scan_damage_gun(8, 12);
	weapon_map["ab"]->icon = "../assets/UI/guns/pistol1.png";

	weapon_map["bc"] = new game::hit_scan_damage_gun(30, 35);
	weapon_map["bc"]->recoil = 3.7f;
	weapon_map["bc"]->cooldown = 0.7f;
	weapon_map["bc"]->scope = 0.3f;
	weapon_map["bc"]->icon = "../assets/UI/guns/riffle1.png";

	weapon_map["cd"] = new game::hit_scan_damage_gun(1, 3);
	weapon_map["cd"]->auto_repeat = true;
	weapon_map["cd"]->recoil = 0.2f;
	weapon_map["cd"]->cooldown = 0.1f;
	weapon_map["cd"]->scope = 0.8f;
	weapon_map["cd"]->icon = "../assets/UI/guns/auto_pistol.png";

	weapon_map["da"] = new game::projectile_damage_gun(16, 19);
	weapon_map["da"]->icon = "../assets/UI/guns/missle_launcher.png";

	game::weapons::throwable_cube = new game::projectile_damage_gun(16, 19);
	game::weapons::throwable_cube->recoil = 0.0f;
	game::weapons::throwable_cube->cooldown = 0.5f;
}

void game::weapon::free()
{
	for (auto w : game::weapon::weapon_map) delete w.second; // free all guns
	delete game::weapons::throwable_cube;
}

