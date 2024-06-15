#include "input_system.h"
#include "ui_slider.h"
#include <glm/ext/matrix_transform.hpp>
#include <ui_system.h>

glm::vec3 ui_system::ui_slider::position3()
{
	return glm::vec3(area.position.x, area.position.y, depth);
}

ui_system::ui_slider::ui_slider(const glm::vec3& position_, const glm::vec2& size_, const glm::vec4& handle_color, const glm::vec4& background_color, const glm::vec4& fill_color) :
	area(position_, size_), depth(position_.z),
	handle("../assets/textures/White_Square.png"),
	background("../assets/textures/White_Square.png"),
	fill("../assets/textures/White_Square.png"),
	handle_base_color(handle_color), handle_hover_color(handle_color), handle_hold_color(handle_color),
	background_base_color(background_color), background_hover_color(background_color), background_hold_color(background_color),
	fill_base_color(fill_color), fill_hover_color(fill_color), fill_hold_color(fill_color)
{
	// colors
	this->handle.color = handle_color;
	this->background.color = background_color;
	this->fill.color = fill_color;

	// color changes
	this->area.on_mouse_enter.subscribe([this](glm::vec2 mp) {
		this->handle.color = handle_hover_color;
		this->background.color = background_hover_color;
		this->fill.color = fill_hover_color;
		});
	this->area.on_mouse_exit.subscribe([this]() {
		this->handle.color = handle_base_color;
		this->background.color = background_base_color;
		this->fill.color = fill_base_color;
		});
	this->area.on_click.subscribe([this]() {
		this->handle.color = handle_hold_color;
		this->background.color = background_hold_color;
		this->fill.color = fill_hold_color;
		});
	this->area.on_release.subscribe([this]() {
		this->handle.color = handle_hover_color;
		this->background.color = background_hover_color;
		this->fill.color = fill_hover_color;
		});

	// value changing
	area.on_drag.subscribe([this](glm::vec2 mp) {
		this->value = (mp.x - this->area.position.x + this->area.size.x) / (this->area.size.x * 2.0f);
		if (this->value > 1.0f) this->value = 1.0f;
		if (this->value < 0.0f) this->value = 0.0f;
		this->update_visual();
		});
	area.on_click.subscribe([this]() {
		this->value = (ui_system::to_ui_space(input_system::mouse_position).x - this->area.position.x + this->area.size.x) / (this->area.size.x * 2.0f);
		if (this->value > 1.0f) this->value = 1.0f;
		if (this->value < 0.0f) this->value = 0.0f;
		this->update_visual();
		});

	//reposition
	this->reposition(this->position3(), area.size);
}

void ui_system::ui_slider::reposition(const glm::vec3& position_, glm::vec2 size_)
{
	this->area.position = position_;
	this->depth = position_.z;
	if (glm::dot(size_, size_) >= 0.0f) {
		this->area.size = size_;
	}
	else size_ = this->area.size;

	this->background.model_matrix = glm::scale(glm::translate(glm::mat4(1.0f), position_), glm::vec3(size_, 1.0f));
	this->update_visual();
}

void ui_system::ui_slider::update_visual()
{
	this->fill.model_matrix = glm::scale(glm::translate(glm::mat4(1.0f), position3() + glm::vec3(area.size.x * (this->value - 1.0f), 0.0f, 0.1f)), glm::vec3(this->value * area.size.x, area.size.y, 1.0f));
	this->handle.model_matrix = glm::scale(glm::translate(glm::mat4(1.0f), position3() + glm::vec3(area.size.x * (this->value * 2.0f - 1.0f), 0.0f, 0.2f)), glm::vec3(area.size.y, area.size.y, 0.0f));
}
