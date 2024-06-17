#include "ui_checkbox.h"
#include <glm/ext/matrix_transform.hpp>

bool ui_system::ui_checkbox::get_value()
{
	return this->check != nullptr;
}

ui_system::ui_checkbox::ui_checkbox(const glm::vec3& position_, const glm::vec2& size_, const std::string& image_, const std::string& check_image_) :
	ui_vbutton(position_, size_), background(image_), check(nullptr), check_image(check_image_),
	background_base_color(glm::vec4(1.0f)), background_hover_color(glm::vec4(0.85f, 0.85f, 0.85f, 1.0f)), background_hold_color(glm::vec4(0.7f, 0.7f, 0.7f, 1.0f)), 
	check_base_color(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)), check_hover_color(glm::vec4(0.0f, 0.85f, 0.0f, 1.0f)), check_hold_color(glm::vec4(0.0f, 0.7f, 0.0f, 1.0f))
{
	this->on_mouse_enter.subscribe([this](glm::vec2 mp) {
		this->background.color = background_hover_color;
		if (this->check) this->check->color = check_hover_color;
		});
	this->on_mouse_exit.subscribe([this]() {
		this->background.color = background_base_color;
		if (this->check) this->check->color = check_base_color;
		});
	this->on_click.subscribe([this]() {
		this->update_check(!(this->get_value()));
		this->background.color = background_hold_color;
		if (this->check) this->check->color = check_hold_color;
		});
	this->on_release.subscribe([this]() {
		this->background.color = background_hover_color;
		if (this->check) this->check->color = check_hover_color;
		});

	//reposition
	this->reposition(this->position3(), this->size);
}

void ui_system::ui_checkbox::reposition(const glm::vec3& position_, const glm::vec2& size_)
{
	this->ui_vbutton::reposition(position_, size_);
	this->background.model_matrix = glm::scale(glm::translate(glm::mat4(1.0f), position_), glm::vec3(this->size, 1.0f));
	if (this->check)
		this->check->model_matrix = this->check_model_matrix();
}

void ui_system::ui_checkbox::update_check(const bool& value)
{
	if (value) {
		if (!(this->check)) {
			this->check = new ui_image(this->check_image, this->check_model_matrix());
			this->check->color = check_base_color;
		}
	}
	else if (this->check) {
		 delete (this->check);
		 this->check = nullptr;
	}
}

ui_system::ui_checkbox::~ui_checkbox()
{
	if (this->check) delete check;
}

glm::mat4 ui_system::ui_checkbox::check_model_matrix()
{
	return glm::scale(glm::translate(glm::mat4(1.0f), this->position3() + glm::vec3(0.0f, 0.0f, 0.1f)), glm::vec3(this->size * 0.85f, 1.0f));
}
