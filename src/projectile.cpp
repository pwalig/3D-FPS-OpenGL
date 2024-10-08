#include "projectile.h"
#include <scripts_system.h>
#include "entity.h"
#include "mesh.h"

game::projectile::projectile(float size) : po(glm::vec3(size)), ft(3.0f, [this]() { this->on_miss(); scripts_system::safe_destroy(this); }) {
	po.name = this->name + "_po";
	po.col.rigidbody = nullptr;
	po.col.owner = this;
	po.col.on_collision_enter.subscribe(std::bind(&game::projectile::hit, this, std::placeholders::_1));
	l.color = glm::vec3(1.4f, 0.7f, 0.7f);
	po.model.mesh = renderer::mesh_ptr("../assets/models/colliders/sphere_collider.mesh");
	po.model.data = renderer::texture_ptr("../assets/textures/White_Square.png");
	po.model.albedo_ = glm::vec3(1.0f, 0.5f, 0.5f);
	po.model.metallic_ = 0.0f;
	po.model.roughness_ = 1.0f;
}

void game::projectile::update()
{
	l.position = this->po.rb.position;
}

void game::projectile::hit(physics::collision_info ci)
{
#ifdef _DEBUG
	if (ci.other->owner != nullptr) printf("%s\n", ci.other->owner->name.c_str());
#endif
	if (game::entity* ent = dynamic_cast<game::entity*>(ci.other->owner)) {
		// we collided with entity
		this->on_hit(ent);
#ifdef _DEBUG
		printf("new hp = %d\n", ent->hp);
#endif
	}
	else this->on_miss();
	scripts_system::safe_destroy(this); // when object deletion happens at loop that caused it -> use safe_destroy, to delay deletion after loop is over
}
