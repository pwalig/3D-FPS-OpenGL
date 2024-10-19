#include "weapon.h"
#include "gameplay_manager.h"
#include "hit_scan_damage_gun.h"
#include "projectile_damage_gun.h"


std::map<std::multiset<game::cube_preset*>, game::weapon*> game::weapon::weapon_map;
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

	// no gun
	w = new weapon();
	w->shoot = [](const glm::vec3& position, const glm::vec3& direction, const int& layer) {};
	w->auto_repeat = false;
	w->recoil = 0.0f;
	w->cooldown = 0.2f;
	w->scope = 0.5f;
	w->icon = "../assets/UI/guns/no_gun.png";
	weapon_map[{}] = w;

	// hand - jumping
	w = new game::projectile_damage_gun(3, 3);
	w->auto_repeat = false;
	w->recoil = 0.3f;
	w->cooldown = 0.3f;
	w->scope = 0.5f;
	w->icon = "../assets/UI/guns/hand.png";
	weapon_map[{&cube_presets::jumping}] = w;

	// knife 1 - speed
	w = new game::projectile_damage_gun(1, 5);
	w->auto_repeat = false;
	w->recoil = 0.0f;
	w->cooldown = 0.45f;
	w->scope = 0.8f;
	w->icon = "../assets/UI/guns/knife1.png";
	weapon_map[{&cube_presets::speed}] = w;

	// pistol 1 - dash
	w = new game::hit_scan_damage_gun(3, 3);
	w->auto_repeat = false;
	w->recoil = 0.2f;
	w->cooldown = 0.25f;
	w->scope = 0.5f;
	w->icon = "../assets/UI/guns/pistol1.png";
	weapon_map[{&cube_presets::dash}] = w;

	// shotgun - missle
	w = new game::hit_scan_damage_gun(0, 15);
	w->auto_repeat = false;
	w->recoil = 3.0f;
	w->cooldown = 0.5f;
	w->scope = 0.65f;
	w->icon = "../assets/UI/guns/shotgun.png";
	weapon_map[{&cube_presets::missle}] = w;

	// missle launcher - jumping/jumping
	w = new game::projectile_damage_gun(16, 19);
	w->auto_repeat = false;
	w->recoil = 0.2f;
	w->cooldown = 0.25f;
	w->scope = 0.5f;
	w->icon = "../assets/UI/guns/missle_launcher.png";
	weapon_map[{&cube_presets::jumping, &cube_presets::jumping}] = w;

	// knives 1 - jumping/speed
	w = new game::projectile_damage_gun(10, 15);
	w->auto_repeat = true;
	w->recoil = 0.0f;
	w->cooldown = 0.2f;
	w->scope = 0.7f;
	w->icon = "../assets/UI/guns/knives1.png";
	weapon_map[{&cube_presets::jumping, &cube_presets::speed}] = w;

	// pistol 3 - jumping/dash
	w = new game::hit_scan_damage_gun(12, 12);
	w->auto_repeat = false;
	w->recoil = 0.3f;
	w->cooldown = 0.3f;
	w->scope = 0.5f;
	w->icon = "../assets/UI/guns/pistol3.png";
	weapon_map[{&cube_presets::jumping, & cube_presets::dash}] = w;

	// riffle 6 - jumping/missle
	w = new game::hit_scan_damage_gun(15, 15);
	w->auto_repeat = false;
	w->recoil = 0.1f;
	w->cooldown = 0.45f;
	w->scope = 0.2f;
	w->icon = "../assets/UI/guns/riffle6.png";
	weapon_map[{&cube_presets::jumping, & cube_presets::missle}] = w;

	// riffle 2 - speed/speed
	w = new game::hit_scan_damage_gun(3, 5);
	w->auto_repeat = true;
	w->recoil = 0.05f;
	w->cooldown = 0.04f;
	w->scope = 0.35f;
	w->icon = "../assets/UI/guns/riffle2.png";
	weapon_map[{&cube_presets::speed, & cube_presets::speed}] = w;

	// auto pistol - speed/dash
	w = new game::hit_scan_damage_gun(1, 3);
	w->auto_repeat = true;
	w->recoil = 0.2f;
	w->cooldown = 0.1f;
	w->scope = 0.8f;
	w->icon = "../assets/UI/guns/auto_pistol.png";
	weapon_map[{&cube_presets::speed, & cube_presets::dash}] = w;

	// pistol 2 - speed/missle
	w = new game::hit_scan_damage_gun(22, 23);
	w->auto_repeat = false;
	w->recoil = 2.5f;
	w->cooldown = 0.65f;
	w->scope = 0.35f;
	w->icon = "../assets/UI/guns/pistol2.png";
	weapon_map[{&cube_presets::speed, & cube_presets::missle}] = w;

	// dual pistol - dash/dash
	w = new game::hit_scan_damage_gun(5, 10);
	w->auto_repeat = true;
	w->recoil = 0.25f;
	w->cooldown = 0.3f;
	w->scope = 0.6f;
	w->icon = "../assets/UI/guns/dual_pistols.png";
	weapon_map[{&cube_presets::dash, & cube_presets::dash}] = w;

	// riffle 1 - dash/missle
	w = new game::hit_scan_damage_gun(30, 35);
	w->auto_repeat = false;
	w->recoil = 3.7f;
	w->cooldown = 0.7f;
	w->scope = 0.3f;
	w->icon = "../assets/UI/guns/riffle1.png";
	weapon_map[{&cube_presets::dash, & cube_presets::missle}] = w;

	// riffle 4 - missle/missle
	w = new game::projectile_damage_gun(40, 50);
	w->auto_repeat = false;
	w->recoil = 4.2f;
	w->cooldown = 1.0f;
	w->scope = 0.35f;
	w->icon = "../assets/UI/guns/riffle4.png";
	weapon_map[{&cube_presets::missle, & cube_presets::missle}] = w;

	// riffle 3 - jumping/jumping/x
	w = new game::hit_scan_damage_gun(60, 70);
	w->auto_repeat = false;
	w->recoil = 3.7f;
	w->cooldown = 0.7f;
	w->scope = 0.3f;
	w->icon = "../assets/UI/guns/riffle3.png";
	weapon_map[{&cube_presets::jumping, & cube_presets::jumping, & cube_presets::jumping}] = w;
	weapon_map[{&cube_presets::jumping, & cube_presets::jumping, & cube_presets::speed}] = new game::weapon(*w);
	weapon_map[{&cube_presets::jumping, & cube_presets::jumping, & cube_presets::dash}] = new game::weapon(*w);
	weapon_map[{&cube_presets::jumping, & cube_presets::jumping, & cube_presets::missle}] = new game::weapon(*w);

	// auto gun 5 - speed/speed/x
	w = new game::hit_scan_damage_gun(10, 10);
	w->auto_repeat = true;
	w->recoil = 0.07f;
	w->cooldown = 0.05f;
	w->scope = 0.4f;
	w->icon = "../assets/UI/guns/auto_gun.png";
	weapon_map[{&cube_presets::speed, & cube_presets::speed, & cube_presets::jumping}] = w;
	weapon_map[{&cube_presets::speed, & cube_presets::speed, & cube_presets::speed}] = new game::weapon(*w);
	weapon_map[{&cube_presets::speed, & cube_presets::speed, & cube_presets::dash}] = new game::weapon(*w);
	weapon_map[{&cube_presets::speed, & cube_presets::speed, & cube_presets::missle}] = new game::weapon(*w);

	// pistol 5 - dash/dash/x
	w = new game::projectile_damage_gun(40, 55);
	w->auto_repeat = false;
	w->recoil = 0.5f;
	w->cooldown = 0.25f;
	w->scope = 0.6f;
	w->icon = "../assets/UI/guns/pistol5.png";
	weapon_map[{&cube_presets::dash, & cube_presets::dash, & cube_presets::jumping}] = w;
	weapon_map[{&cube_presets::dash, & cube_presets::dash, & cube_presets::speed}] = new game::weapon(*w);
	weapon_map[{&cube_presets::dash, & cube_presets::dash, & cube_presets::dash}] = new game::weapon(*w);
	weapon_map[{&cube_presets::dash, & cube_presets::dash, & cube_presets::missle}] = new game::weapon(*w);

	// riffle 7 - missle/missle/x
	w = new game::projectile_damage_gun(70, 80);
	w->auto_repeat = false;
	w->recoil = 4.0f;
	w->cooldown = 1.1f;
	w->scope = 0.4f;
	w->icon = "../assets/UI/guns/riffle7.png";
	weapon_map[{&cube_presets::missle, & cube_presets::missle, & cube_presets::jumping}] = w;
	weapon_map[{&cube_presets::missle, & cube_presets::missle, & cube_presets::speed}] = new game::weapon(*w);
	weapon_map[{&cube_presets::missle, & cube_presets::missle, & cube_presets::dash}] = new game::weapon(*w);
	weapon_map[{&cube_presets::missle, & cube_presets::missle, & cube_presets::missle}] = new game::weapon(*w);

	// gun1 - jumping/missle/x
	w = new game::projectile_damage_gun(55, 55);
	w->auto_repeat = false;
	w->recoil = 1.5f;
	w->cooldown = 0.5f;
	w->scope = 0.7f;
	w->icon = "../assets/UI/guns/gun1.png";
	weapon_map[{&cube_presets::jumping, & cube_presets::missle, & cube_presets::dash}] = w;
	weapon_map[{&cube_presets::jumping, & cube_presets::missle, & cube_presets::speed}] = new game::weapon(*w);

	// riffle5 - speed/dash/x
	w = new game::hit_scan_damage_gun(49, 51);
	w->auto_repeat = true;
	w->recoil = 1.8f;
	w->cooldown = 0.4f;
	w->scope = 0.35f;
	w->icon = "../assets/UI/guns/riffle5.png";
	weapon_map[{&cube_presets::speed, & cube_presets::dash, & cube_presets::jumping}] = w;
	weapon_map[{&cube_presets::speed, & cube_presets::dash, & cube_presets::missle}] = new game::weapon(*w);

	game::weapons::throwable_cube = new game::projectile_damage_gun(16, 19);
	game::weapons::throwable_cube->recoil = 0.0f;
	game::weapons::throwable_cube->cooldown = 0.5f;
}

void game::weapon::free()
{
	for (auto w : game::weapon::weapon_map) delete w.second; // free all guns
	delete game::weapons::throwable_cube;
}

