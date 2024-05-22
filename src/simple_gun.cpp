#include "simple_gun.h"
#include "physics.h"
#include <damage_number.h>
#include <entity.h>

game::simple_gun::simple_gun()
{
	this->on_hit = [this](game::entity* ent) {
		ent->damage(this->damage);
		game::damage_number* dm = new game::damage_number(10);
		dm->uit.color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
		};

	this->on_miss = []() {
		game::damage_number* dm = new game::damage_number(0);
		dm->uit.color = glm::vec4(0.7f, 0.7f, 0.7f, 0.5f);
		};

	this->shoot = [this](const glm::vec3& position, const glm::vec3& direction, const int& layer) {
		weapon::hit_scan(physics::ray(position, direction, layer), this->on_hit, this->on_miss);
		};
}
