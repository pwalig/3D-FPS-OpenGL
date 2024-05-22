#include "projectile.h"
#include <scripts_system.h>
#include "entity.h"

game::projectile::projectile(float size) : po(glm::vec3(size)), damage(10) {
	po.name = this->name + "_po";
	po.col.rigidbody = nullptr;
	po.col.on_collision_enter.subscribe(std::bind(&game::projectile::hit, this, std::placeholders::_1));
	t.events.subscribe([this]() {printf("gone\n"); scripts_system::safe_destroy(this); }); // expire
	t.start(3.0f);
}

void game::projectile::update()
{}

void game::projectile::hit(physics::collision_info ci)
{
	if (ci.other->owner != nullptr) printf("%s\n", ci.other->owner->name.c_str());
	if (game::entity* ent = dynamic_cast<game::entity*>(ci.other->owner)) {
		// we collided with entity
		ent->damage(this->damage);
		printf("new hp = %d\n", ent->hp);
	}
	scripts_system::safe_destroy(this); // when object deletion happens at loop that caused it -> use safe_destroy, to delay deletion after loop is over
}
