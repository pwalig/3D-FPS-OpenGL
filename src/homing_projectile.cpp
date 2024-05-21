#include "homing_projectile.h"

void game::homing_projectile::homing_none(homing_projectile* hproj, const glm::vec3& target)
{
}

void game::homing_projectile::homing_force_normalized(homing_projectile* hproj, const glm::vec3& target)
{
	hproj->po.rb.temp_force = glm::normalize(hproj->target - hproj->po.rb.position) * hproj->homing_force * hproj->po.rb.mass;
}

void game::homing_projectile::homing_force_spring(homing_projectile* hproj, const glm::vec3& target)
{
	hproj->po.rb.temp_force = (hproj->target - hproj->po.rb.position) * hproj->homing_force * hproj->po.rb.mass;
}

void game::homing_projectile::homing_force_gravity(homing_projectile* hproj, const glm::vec3& target)
{
	float dist = glm::length(hproj->target - hproj->po.rb.position);
	hproj->po.rb.temp_force = glm::normalize(hproj->target - hproj->po.rb.position) * hproj->homing_force * hproj->po.rb.mass / (dist * dist);
}

void game::homing_projectile::homing_rotation_normalized(homing_projectile* hproj, const glm::vec3& target)
{
	glm::vec3 dir = glm::normalize(hproj->target - hproj->po.rb.position);
	glm::vec3 cfor = hproj->po.rb.rotation * glm::vec3(0.0f, 0.0f, 1.0f);
	float angle = glm::acos(glm::dot(cfor, dir));
	if (angle > 0.0f) angle = hproj->homing_force;
	else if (angle < 0.0f) angle = -hproj->homing_force;
	hproj->po.rb.rotation = glm::rotate(hproj->po.rb.rotation, angle, glm::normalize(glm::cross(cfor, dir)));
	hproj->po.rb.velocity = hproj->po.rb.rotation * glm::vec3(0.0f, 0.0f, glm::length(hproj->po.rb.velocity));
}

void game::homing_projectile::homing_rotation_spring(homing_projectile* hproj, const glm::vec3& target)
{
	glm::vec3 dir = glm::normalize(hproj->target - hproj->po.rb.position);
	glm::vec3 cfor = hproj->po.rb.rotation * glm::vec3(0.0f, 0.0f, 1.0f);
	hproj->po.rb.rotation = glm::rotate(hproj->po.rb.rotation, glm::acos(glm::dot(cfor, dir)) * hproj->homing_force, glm::normalize(glm::cross(cfor, dir)));
	hproj->po.rb.velocity = hproj->po.rb.rotation * glm::vec3(0.0f, 0.0f, glm::length(hproj->po.rb.velocity));
}

void game::homing_projectile::homing_velocity_normalized(homing_projectile* hproj, const glm::vec3& target)
{
	hproj->po.rb.velocity = glm::normalize(hproj->target - hproj->po.rb.position) * hproj->homing_force;
}

void game::homing_projectile::homing_velocity_spring(homing_projectile* hproj, const glm::vec3& target)
{
	hproj->po.rb.velocity = (hproj->target - hproj->po.rb.position) * hproj->homing_force;
}

game::homing_projectile::homing_projectile(glm::vec3& target_, const float& size) : projectile(size), target(target_){}

void game::homing_projectile::update()
{
	this->homing_function(this, this->target);
}
