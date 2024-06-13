#include "hit_scan_damage_gun.h"
#include "physics.h"
#include <damage_number.h>
#include <entity.h>
#include "random_t.h"
#include <light.h>
#include <scripts_system.h>
#include <time_system.h>

game::hit_scan_damage_gun::hit_scan_damage_gun(const int& max_damage_, const int& min_damage_) : max_damage(max_damage_), min_damage(min_damage_)
{
	this->shoot = [this](const glm::vec3& position, const glm::vec3& direction, const int& layer) {
		//light
		renderer::light* l = new renderer::light(position, glm::vec3(2.0f, 1.0f, 0.9f));
		time_system::function_timer* ft = new time_system::function_timer();
		unsigned int id = scripts_system::events[SCRIPTS_UPDATE].subscribe([l, this, direction]() {
			l->position += direction * (float)time_system::delta_time * 400.0f;
			l->color -= (glm::vec3(2.0f, 1.0f, 0.9f) * (float)time_system::delta_time) / this->cooldown;
			}
		);
		ft->function = [l, id]() {
			//printf("lights timr elaps: %d\n", renderer::light::all_lights.size());
			scripts_system::events[SCRIPTS_UPDATE].unsubscribe(id);
			delete l;
			};
		ft->die_on_finish = true;
		ft->start(this->cooldown);
		//printf("lights timr strt: %d\n", renderer::light::all_lights.size());

		// hit scan
		weapon::hit_scan(
			physics::ray(position, direction, layer), // ray
			[this](game::entity* ent) { // on hit
				int damage = RandomT<int>(this->min_damage, this->max_damage);
				ent->damage(damage);
				game::damage_number* dm = new game::damage_number(damage);
				dm->uit.color = glm::vec4((float)damage / (float)max_damage, 0.0f, 0.0f, 1.0f);
			},
			[]() { // on_miss
				game::damage_number* dm = new game::damage_number(0);
				dm->uit.color = glm::vec4(0.7f, 0.7f, 0.7f, 0.5f);
			});
		};
}
