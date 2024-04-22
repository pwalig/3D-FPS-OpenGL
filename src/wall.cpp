#include "wall.h"
#include "renderer.h"

game::wall::wall(const glm::vec3& size) : rb(), col(&rb, glm::vec2(size.x, size.z))
{
	rb.restitution = 0.0f;
	rb.dynamic = false;

	renderer::model m1;
	m1.model_matrix = glm::scale(rb.model_matrix(), size / 2.0f);
	renderer::all_models.push_back(m1);
}

void game::wall::update()
{
	//printf("w_pos %f %f %f\n", rb.position.x, rb.position.y, rb.position.z);
	//printf("w_velo %f %f %f\n", rb.velocity.x, rb.velocity.y, rb.velocity.z);
	//printf("W_force %f %f %f\n", rb.force.x + rb.temp_force.x, rb.force.y + rb.temp_force.y, rb.force.z + rb.temp_force.z);
}
