#include "ui_slider.h"
#include <glm/ext/matrix_transform.hpp>
#include <time_system.h>

ui_system::ui_slider::ui_slider(const glm::vec3& position_, const glm::vec2& size_, const glm::vec4& handle_color, const glm::vec4& background_color, const glm::vec4& fill_color) :
	position(position_), size(size_), handle(position_ + glm::vec3(-size_.x, 0.0f, 0.2f), glm::vec2(size_.y, size_.y), handle_color),
	background("../assets/textures/White_Square.png"),
	fill("../assets/textures/White_Square.png")
{
	this->reposition(position, size);
	handle.on_drag.subscribe([this](glm::vec2 mp) {
		this->value = (mp.x - this->position.x + this->size.x) / (this->size.x * 2.0f);
		if (this->value > 1.0f) this->value = 1.0f;
		if (this->value < 0.0f) this->value = 0.0f;
		this->update_visual();
		});
}

void ui_system::ui_slider::reposition(const glm::vec3& position_, glm::vec2 size_)
{
	this->position = position_;
	if (glm::dot(size_, size_) >= 0.0f) {
		this->size = size_;
	}
	else size_ = this->size;

	this->background.model_matrix = glm::scale(glm::translate(glm::mat4(1.0f), position_), glm::vec3(size_, 1.0f));
	this->update_visual();
}

void ui_system::ui_slider::update_visual()
{
	this->fill.model_matrix = glm::scale(glm::translate(glm::mat4(1.0f), position + glm::vec3(size.x * (this->value - 1.0f), 0.0f, 0.1f)), glm::vec3(this->value * size.x, size.y, 1.0f));
	this->handle.reposition(position + glm::vec3(size.x * (this->value * 2.0f - 1.0f), 0.0f, 0.2f), glm::vec2(size.y, size.y));
}
