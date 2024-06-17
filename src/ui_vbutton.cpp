#include "ui_vbutton.h"
#include <glm/ext/matrix_transform.hpp>

glm::vec3 ui_system::ui_vbutton::position3()
{
	return glm::vec3(this->position, depth);
}

ui_system::ui_vbutton::ui_vbutton(const glm::vec3& position_, const glm::vec2& size_) : ui_button(position_, size_), depth(position_.z)
{
	this->reposition(position_, size_);
}

void ui_system::ui_vbutton::reposition(const glm::vec3& position_, const glm::vec2& size_)
{
	this->position = position_;
	if (glm::dot(size_, size_) >= 0.0f) {
		this->size = size_;
	}
}
