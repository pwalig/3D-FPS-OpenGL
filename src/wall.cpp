#include "wall.h"
#include "renderer.h"

game::wall::wall(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& size) : rb(), col(&rb, size)
{
	rb.restitution = 0.0f;
	rb.position = position;
	rb.rotation = glm::quat(rotation);
	rb.dynamic = false;

	renderer::model m1;
	m1.model_matrix = glm::translate(rb.model_matrix(), glm::vec3(0.0f, -col.size.y / 2.0f, 0.0f));
	m1.model_matrix = glm::scale(m1.model_matrix, glm::vec3(size.x / 2.0f, size.y, size.z / 2.0f));
	renderer::all_models.push_back(m1);
}
