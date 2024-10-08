#include "ui_text_button.h"
#include <glm/ext/matrix_transform.hpp>

void ui_system::ui_text_button::update_text(const std::string& new_text)
{
	this->text.update_text(new_text);
	this->text.model_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(this->size.x * 2.0f / this->text.text.size(), this->size.y * 2.0f, 1.0f));
}

ui_system::ui_text_button::ui_text_button(
	const glm::vec3& position_, const glm::vec2& size_, const std::string& image_,
	const std::string& text_, const std::string& font,
	const glm::vec4& base_color_, const glm::vec4& hover_color_, const glm::vec4& hold_color_
) :
	ui_button(position_, size_),
	image(image_, position_), text(text_, font.c_str(), position_),
	base_color(base_color_), hover_color(hover_color_), hold_color(hold_color_)
{
	this->image.color = this->base_color;
	this->text.color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

	this->on_mouse_enter.subscribe([this](glm::vec2 mp) {
		this->image.color = hover_color;
		});
	this->on_mouse_exit.subscribe([this]() {
		this->image.color = base_color;
		});
	this->on_click.subscribe([this]() {
		this->image.color = hold_color;
		});
	this->on_release.subscribe([this]() {
		this->image.color = hover_color;
		});

	//reposition
	this->reposition(this->position, this->size);
}

void ui_system::ui_text_button::reposition(const glm::vec3& position_, const glm::vec2& size_)
{
	this->ui_button::reposition(position_, size_);
	this->image.anchor_point = position_;
	this->image.model_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(this->size, 1.0f));
	this->text.anchor_point = position_;
	this->text.anchor_point.z -= 0.001f;
	this->text.model_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(this->size.x * 2.0f / this->text.text.size(), this->size.y * 2.0f, 1.0f));
}
