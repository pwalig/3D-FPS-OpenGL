#include "projectile_damage_gun.h"
#include <projectile.h>
#include "scripts_system.h"
#include "random_t.h"
#include <damage_number.h>

game::projectile_damage_gun::projectile_damage_gun(const int& max_damage_, const int& min_damage_) : max_damage(max_damage_), min_damage(min_damage_)
{
	this->shoot = [this](const glm::vec3& position, const glm::vec3& direction, const int& layer) {
		game::projectile* proj = scripts_system::instantiate<game::projectile, float>(0.15f);
		proj->on_hit = [this](game::entity* ent) {
			int damage = RandomT<int>(this->min_damage, this->max_damage);
			ent->damage(damage);
			game::damage_number* dm = new game::damage_number(damage);
			dm->uit.color = glm::vec4((float)damage / (float)max_damage, 0.0f, 0.0f, 1.0f);
			};
		proj->on_miss = []() {
			game::damage_number* dm = new game::damage_number(0);
			dm->uit.color = glm::vec4(0.7f, 0.7f, 0.7f, 0.5f);
			};
		proj->po.rb.position = position;
		proj->po.rb.velocity = direction * 5.0f;
		proj->po.rb.mass = 0.1f;
		proj->po.rb.force = glm::normalize(direction) * 10.0f;
		proj->po.rb.restitution = 0.0f;
		proj->po.col.radius = 0.15f;
		proj->po.col.layer = layer;
		};
}
