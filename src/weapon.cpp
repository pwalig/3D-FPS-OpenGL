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
	game::weapon* w; // helper variable for temporarily storing weapons

	w = new game::hit_scan_damage_gun(12, 12);
	w->auto_repeat = false;
	w->recoil = 0.3f;
	w->cooldown = 0.3f;
	w->scope = 0.5f;
	w->icon = "../assets/UI/guns/pistol3.png";
	weapon_map[{&cube_presets::jumping, &cube_presets::jumping}] = w;

	w = new game::hit_scan_damage_gun(8, 12);
	w->auto_repeat = false;
	w->recoil = 0.2f;
	w->cooldown = 0.25f;
	w->scope = 0.5f;
	w->icon = "../assets/UI/guns/pistol1.png";
	weapon_map[{&cube_presets::jumping, &cube_presets::speed}] = w;

	w = new game::hit_scan_damage_gun(20, 25);
	w->auto_repeat = false;
	w->recoil = 3.0f;
	w->cooldown = 0.4f;
	w->scope = 0.6f;
	w->icon = "../assets/UI/guns/shotgun.png";
	weapon_map[{&cube_presets::jumping, & cube_presets::dash}] = w;

	w = new game::projectile_damage_gun(16, 19);
	w->auto_repeat = false;
	w->recoil = 0.2f;
	w->cooldown = 0.25f;
	w->scope = 0.5f;
	w->icon = "../assets/UI/guns/missle_launcher.png";
	weapon_map[{&cube_presets::jumping, & cube_presets::missle}] = w;

	w = new game::hit_scan_damage_gun(3, 5);
	w->auto_repeat = true;
	w->recoil = 0.05f;
	w->cooldown = 0.04f;
	w->scope = 0.35f;
	w->icon = "../assets/UI/guns/riffle2.png";
	weapon_map[{&cube_presets::speed, & cube_presets::speed}] = w;

	w = new game::hit_scan_damage_gun(30, 35);
	w->auto_repeat = false;
	w->recoil = 3.7f;
	w->cooldown = 0.7f;
	w->scope = 0.3f;
	w->icon = "../assets/UI/guns/riffle1.png";
	weapon_map[{&cube_presets::speed, & cube_presets::dash}] = w;

	w = new game::hit_scan_damage_gun(22, 23);
	w->auto_repeat = false;
	w->recoil = 2.5f;
	w->cooldown = 0.65f;
	w->scope = 0.35f;
	w->icon = "../assets/UI/guns/pistol2.png";
	weapon_map[{&cube_presets::speed, & cube_presets::missle}] = w;

	w = new game::projectile_damage_gun(40, 50);
	w->auto_repeat = false;
	w->recoil = 4.2f;
	w->cooldown = 1.0f;
	w->scope = 0.35f;
	w->icon = "../assets/UI/guns/riffle4.png";
	weapon_map[{&cube_presets::dash, & cube_presets::dash}] = w;

	w = new game::hit_scan_damage_gun(1, 3);
	w->auto_repeat = true;
	w->recoil = 0.2f;
	w->cooldown = 0.1f;
	w->scope = 0.8f;
	w->icon = "../assets/UI/guns/auto_pistol.png";
	weapon_map[{&cube_presets::dash, & cube_presets::missle}] = w;

	w = new game::hit_scan_damage_gun(5, 10);
	w->auto_repeat = true;
	w->recoil = 0.25f;
	w->cooldown = 0.3f;
	w->scope = 0.6f;
	w->icon = "../assets/UI/guns/dual_pistols.png";
	weapon_map[{&cube_presets::missle, & cube_presets::missle}] = w;

	game::weapons::throwable_cube = new game::projectile_damage_gun(16, 19);
	game::weapons::throwable_cube->recoil = 0.0f;
	game::weapons::throwable_cube->cooldown = 0.5f;
}

void game::weapon::free()
{
	for (auto w : game::weapon::weapon_map) delete w.second; // free all guns
	delete game::weapons::throwable_cube;
}

