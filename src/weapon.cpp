#include "weapon.h"
#include "gameplay_manager.h"
#include "hit_scan_damage_gun.h"
#include "projectile_damage_gun.h"


std::map<std::set<game::cube_preset*>, game::weapon*> game::weapon::weapon_map;
game::weapon* game::weapons::throwable_cube = nullptr;

void game::weapon::hit_scan(const physics::ray& r, const std::function<void(game::entity*)>& on_hit, const std::function<void()>& on_miss)
{
	if (gameplay_manager::game_paused) return;
	physics::ray_intersection_info ri = physics::ray_cast(r);
	if (ri.intersect == RAY_INTERSECT_NONE) {
		on_miss();
#ifdef _DEBUG
		printf("no ray hit\n");
#endif
		return;
	}
#ifdef _DEBUG
	if (ri.col->owner) printf("ri: %s, %d, %f\n", ri.col->owner->name.c_str(), ri.intersect, ri.distance);
#endif
	if (game::entity* ent = dynamic_cast<game::entity*>(ri.col->owner)) {
		// we collided with entity
		on_hit(ent);

#ifdef _DEBUG
		printf("new hp = %d\n", ent->hp);
#endif
	}
	else {
		on_miss();
	}
}

void game::weapon::init()
{
	weapon_map[{&cube_presets::jumping, &cube_presets::speed}] = new game::hit_scan_damage_gun(8, 12);
	weapon_map[{&cube_presets::jumping, &cube_presets::speed}]->auto_repeat = false;
	weapon_map[{&cube_presets::jumping, &cube_presets::speed}]->recoil = 0.2f;
	weapon_map[{&cube_presets::jumping, &cube_presets::speed}]->cooldown = 0.25f;
	weapon_map[{&cube_presets::jumping, &cube_presets::speed}]->scope = 0.5f;
	weapon_map[{&cube_presets::jumping, &cube_presets::speed}]->icon = "../assets/UI/guns/pistol1.png";

	weapon_map[{&cube_presets::jumping, &cube_presets::dash}] = new game::hit_scan_damage_gun(20, 25);
	weapon_map[{&cube_presets::jumping, &cube_presets::dash}]->auto_repeat = false;
	weapon_map[{&cube_presets::jumping, &cube_presets::dash}]->recoil = 3.0f;
	weapon_map[{&cube_presets::jumping, &cube_presets::dash}]->cooldown = 0.4f;
	weapon_map[{&cube_presets::jumping, &cube_presets::dash}]->scope = 0.6f;
	weapon_map[{&cube_presets::jumping, &cube_presets::dash}]->icon = "../assets/UI/guns/shotgun.png";

	weapon_map[{&cube_presets::jumping, &cube_presets::missle}] = new game::projectile_damage_gun(16, 19);
	weapon_map[{&cube_presets::jumping, &cube_presets::missle}]->auto_repeat = false;
	weapon_map[{&cube_presets::jumping, &cube_presets::missle}]->recoil = 0.2f;
	weapon_map[{&cube_presets::jumping, &cube_presets::missle}]->cooldown = 0.25f;
	weapon_map[{&cube_presets::jumping, &cube_presets::missle}]->scope = 0.5f;
	weapon_map[{&cube_presets::jumping, &cube_presets::missle}]->icon = "../assets/UI/guns/missle_launcher.png";

	weapon_map[{&cube_presets::speed, &cube_presets::dash}] = new game::hit_scan_damage_gun(30, 35);
	weapon_map[{&cube_presets::speed, &cube_presets::dash}]->auto_repeat = false;
	weapon_map[{&cube_presets::speed, &cube_presets::dash}]->recoil = 3.7f;
	weapon_map[{&cube_presets::speed, &cube_presets::dash}]->cooldown = 0.7f;
	weapon_map[{&cube_presets::speed, &cube_presets::dash}]->scope = 0.3f;
	weapon_map[{&cube_presets::speed, &cube_presets::dash}]->icon = "../assets/UI/guns/riffle1.png";

	weapon_map[{&cube_presets::speed, &cube_presets::missle}] = new game::hit_scan_damage_gun(22, 23);
	weapon_map[{&cube_presets::speed, &cube_presets::missle}]->auto_repeat = false;
	weapon_map[{&cube_presets::speed, &cube_presets::missle}]->recoil = 2.5f;
	weapon_map[{&cube_presets::speed, &cube_presets::missle}]->cooldown = 0.65f;
	weapon_map[{&cube_presets::speed, &cube_presets::missle}]->scope = 0.35f;
	weapon_map[{&cube_presets::speed, &cube_presets::missle}]->icon = "../assets/UI/guns/pistol2.png";

	weapon_map[{&cube_presets::dash, &cube_presets::missle}] = new game::hit_scan_damage_gun(1, 3);
	weapon_map[{&cube_presets::dash, &cube_presets::missle}]->auto_repeat = true;
	weapon_map[{&cube_presets::dash, &cube_presets::missle}]->recoil = 0.2f;
	weapon_map[{&cube_presets::dash, &cube_presets::missle}]->cooldown = 0.1f;
	weapon_map[{&cube_presets::dash, &cube_presets::missle}]->scope = 0.8f;
	weapon_map[{&cube_presets::dash, &cube_presets::missle}]->icon = "../assets/UI/guns/auto_pistol.png";

	game::weapons::throwable_cube = new game::projectile_damage_gun(16, 19);
	game::weapons::throwable_cube->recoil = 0.0f;
	game::weapons::throwable_cube->cooldown = 0.5f;
}

void game::weapon::free()
{
	for (auto w : game::weapon::weapon_map) delete w.second; // free all guns
	delete game::weapons::throwable_cube;
}

