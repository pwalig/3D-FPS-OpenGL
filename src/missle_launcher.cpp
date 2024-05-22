#include "missle_launcher.h"
#include <projectile.h>
#include "scripts_system.h"

void game::missle_launcher::shoot(const glm::vec3& position, const glm::vec3& direction, const int& layer)
{
	game::projectile* proj = scripts_system::instantiate<game::projectile, float>(0.15f, this);
	proj->damage = 10;
	proj->po.rb.position = position;
	proj->po.rb.velocity = direction * 5.0f;
	proj->po.rb.mass = 0.1f;
	proj->po.rb.force = glm::normalize(direction) * 10.0f;
	proj->po.rb.restitution = 0.0f;
	proj->po.col.radius = 0.15f;
	proj->po.col.layer = layer;
}
