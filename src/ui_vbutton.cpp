#include "ui_vbutton.h"
#include <glm/ext/matrix_transform.hpp>

ui_system::ui_vbutton::ui_vbutton(const std::string& image, const std::string& text_, const std::string& font, const glm::vec4& color, const glm::vec3& position_, const glm::vec2& size_) :
	ui_button(position_, size_),
	base_image(renderer::get_texture(image)), hover_image(renderer::get_texture(image)), hold_image(renderer::get_texture(image)),
	text(text_, font.c_str()),
	base_color(color), hover_color(color), hold_color(color),
	image(image.c_str())
{
	this->reposition(position_, size_);
	this->on_mouse_enter.subscribe([this](glm::vec2 mp) {
		this->image.texture = hover_image;
		this->image.color = hover_color;
		});
	this->on_mouse_exit.subscribe([this]() {
		this->image.texture = base_image;
		this->image.color = base_color;
		});
	this->on_click.subscribe([this]() {
		this->image.texture = hold_image;
		this->image.color = hold_color;
		});
	this->on_release.subscribe([this]() {
		this->image.texture = hover_image;
		this->image.color = hover_color;
		});
}

void ui_system::ui_vbutton::reposition(const glm::vec3& position_, glm::vec2 size_)
{
	this->position = position_;
	if (glm::dot(size_, size_) >= 0.0f) {
		this->size = size_;
	}
	else size_ = this->size;

	this->image.model_matrix = glm::scale(glm::translate(glm::mat4(1.0f), position_), glm::vec3(size_, 1.0f));
	text.model_matrix = glm::scale(glm::translate(glm::mat4(1.0f), position_ + glm::vec3(-size_.x, size_.y, 0.1f)), glm::vec3(size_.x * 2.0f / this->text.text.size(), size_.y * 2.0f, 1.0f));
}
