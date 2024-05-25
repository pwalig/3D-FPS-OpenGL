#include "hit_scan_damage_gun.h"
#include "physics.h"
#include <damage_number.h>
#include <entity.h>
#include "random_t.h"

game::hit_scan_damage_gun::hit_scan_damage_gun(const int& max_damage_, const int& min_damage_) : max_damage(max_damage_), min_damage(min_damage_)
{
	this->on_hit = [this](game::entity* ent) {
		int damage = RandomT<int>(this->min_damage, this->max_damage);
		ent->damage(damage);
		game::damage_number* dm = new game::damage_number(damage);
		dm->uit.color = glm::vec4((float)damage / (float)max_damage, 0.0f, 0.0f, 1.0f);
		};

	this->on_miss = []() {
		game::damage_number* dm = new game::damage_number(0);
		dm->uit.color = glm::vec4(0.7f, 0.7f, 0.7f, 0.5f);
		};

	this->shoot = [this](const glm::vec3& position, const glm::vec3& direction, const int& layer) {
		weapon::hit_scan(physics::ray(position, direction, layer), this->on_hit, this->on_miss);
		};
}
