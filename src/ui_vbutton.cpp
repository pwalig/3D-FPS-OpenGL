#include "ui_vbutton.h"
#include <glm/ext/matrix_transform.hpp>

ui_system::ui_vbutton::ui_vbutton(const std::string& image, const std::string& text_, const std::string& font, const glm::vec4& color, const glm::vec2& position_, const glm::vec2& size_) :
	ui_button(position_, size_),
	base_image(renderer::get_texture(image)), hover_image(renderer::get_texture(image)), hold_image(renderer::get_texture(image)),
	text(text_, font.c_str(), glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(position_, -10.0f)), glm::vec3(size_, 1.0f))),
	base_color(color), hover_color(color), hold_color(color),
	image(image.c_str(), glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(position_, -11.0f)), glm::vec3(size_, 1.0f)))
{
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
