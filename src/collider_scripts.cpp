#include "collider_scripts.h"


game::colliders::base::base(const glm::vec3& position, const std::string& mesh, const glm::quat& rotation)// : model(mesh, glm::vec4(0,1,0,0.1f))
{
	rb.position = position;
	rb.rotation = rotation;
	rb.dynamic = false;
	//model.model_matrix = rb.model_matrix();
	rb.restitution = 0.0f;
}

game::colliders::sphere::sphere(const glm::vec3& position, const float& radius) : base(position, "../assets/models/colliders/sphere_collider.mesh"), col(&rb, this, radius)
{
	//model.model_matrix = glm::scale(model.model_matrix, glm::vec3(radius));
}

game::colliders::aabb::aabb(const glm::vec3& position, const glm::vec3& size) : base(position, "../assets/models/colliders/box_collider.mesh"), col(&rb, this, size)
{
	//model.model_matrix = glm::scale(model.model_matrix, size);
}

game::colliders::box::box(const glm::vec3& position, const glm::quat& rotation, const glm::vec3& size) : base(position, "../assets/models/colliders/box_collider.mesh", rotation), col(&rb, this, size)
{
	//model.model_matrix = glm::scale(model.model_matrix, size);
}

game::colliders::capsule::capsule(const glm::vec3& position, const glm::quat& rotation, const float& radius, const float& spread) : base(position, "../assets/models/capsule_collider.mesh", rotation), col(&rb, this, radius, spread)
{
	//model.model_matrix = glm::scale(model.model_matrix, glm::vec3(radius, spread, radius));
}

game::colliders::plane::plane(const glm::vec3& position, const glm::quat& rotation, const glm::vec3& size) : base(position, "../assets/models/colliders/plane_collider.mesh", rotation), col(&rb, this, size)
{
	//model.model_matrix = glm::scale(model.model_matrix, size);
}
