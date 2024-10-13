#include "ui_draggable.h"

ui_system::ui_draggable::ui_draggable(
	const glm::vec3& position_,
	const glm::vec2& size_,
	ui_system::ui_visual* content_
): ui_button(position_, size_), content(content_)
{
	this->on_drag.subscribe(std::bind(&ui_system::ui_draggable::update_position, this, std::placeholders::_1));
	if (this->content) this->content->anchor_point = position_;
}

void ui_system::ui_draggable::update_position(const glm::vec2& position)
{
	glm::vec3 pos3 = glm::vec3(position.x, position.y, this->position.z);
	this->reposition(pos3);
	if (this->content) content->anchor_point = pos3;
}
