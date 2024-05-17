#include "bullet.h"
#include <scripts_system.h>

game::bullet::bullet(float size) : po(glm::vec3(size)), damage(0.0f) {
	po.col.rigidbody = nullptr;
	po.col.on_collision_enter.subscribe(std::bind(&game::bullet::hit, this, std::placeholders::_1));
	t.events.subscribe([this]() {printf("gone\n"); scripts_system::safe_destroy(this); }); // expire
	t.start(3.0f);
}

void game::bullet::update()
{
	this->po.update();
}

void game::bullet::hit(physics::collision_info ci)
{
	printf("boom\n");
	scripts_system::safe_destroy(this); // when object deletion happens at loop that caused it -> use safe_destroy, to delay deletion after loop is over
}
