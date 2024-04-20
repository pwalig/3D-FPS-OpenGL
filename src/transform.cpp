#include "transform.h"

glm::mat4 engine::transform::global_model_matrix() const
{
	// calculate model matrix
	glm::mat4 M = glm::translate(glm::mat4(1.0f), this->local_position);
	M *= glm::toMat4(this->local_rotation);
	M = glm::scale(M, this->local_scale);
	if (this->parent != nullptr) return this->parent->global_model_matrix() * M;
	else return M;
}
