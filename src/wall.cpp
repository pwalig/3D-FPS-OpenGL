#include "wall.h"
#include "renderer.h"

game::wall::wall(const glm::vec3& size) : rb(), col(&rb, glm::vec2(size.x, size.z))
{
	rb.restitution = 0.0f;
	physics::all_colliders.push_back(&col);

	renderer::model m1;
	m1.model_matrix = glm::scale(rb.model_matrix(), size / 2.0f);
	renderer::all_models.push_back(m1);
}
