#include "input_system.h"
#include "ui_slider.h"
#include <glm/ext/matrix_transform.hpp>
#include <ui_system.h>

ui_system::ui_slider::ui_slider(const glm::vec3& position_, const glm::vec2& size_, const glm::vec4& handle_color, const glm::vec4& background_color, const glm::vec4& fill_color) :
	ui_button(position_, size_),
	handle("../assets/textures/White_Square.png"),
	background("../assets/textures/White_Square.png"),
	fill("../assets/textures/White_Square.png"),
	handle_base_color(handle_color), handle_hover_color(handle_color), handle_hold_color(handle_color),
	background_base_color(background_color), background_hover_color(background_color), background_hold_color(background_color),
	fill_base_color(fill_color), fill_hover_color(fill_color), fill_hold_color(fill_color)
{
	// fill pivot
	fill.pivot_point = glm::vec3(-1.0f, 0.0f, 0.0f);

	// colors
	this->handle.color = handle_color;
	this->background.color = background_color;
	this->fill.color = fill_color;

	// color changes
	this->on_mouse_enter.subscribe([this](glm::vec2 mp) {
		this->handle.color = handle_hover_color;
		this->background.color = background_hover_color;
		this->fill.color = fill_hover_color;
		});
	this->on_mouse_exit.subscribe([this]() {
		this->handle.color = handle_base_color;
		this->background.color = background_base_color;
		this->fill.color = fill_base_color;
		});
	this->on_click.subscribe([this]() {
		this->handle.color = handle_hold_color;
		this->background.color = background_hold_color;
		this->fill.color = fill_hold_color;
		});
	this->on_release.subscribe([this]() {
		this->handle.color = handle_hover_color;
		this->background.color = background_hover_color;
		this->fill.color = fill_hover_color;
		});

	// value changing
	this->on_drag.subscribe([this](glm::vec2 mp) {
		float x = this->scaled_size().x;
		this->value = (mp.x - this->position.x + x) / (x * 2.0f);
		if (this->value > 1.0f) this->value = 1.0f;
		if (this->value < 0.0f) this->value = 0.0f;
		this->update_visual();
		this->on_value_changed(this->value);
		});
	this->on_click.subscribe([this]() {
		float x = this->scaled_size().x;
		this->value = (ui_system::to_ui_space(input_system::mouse_position).x - this->position.x + x) / (x * 2.0f);
		if (this->value > 1.0f) this->value = 1.0f;
		if (this->value < 0.0f) this->value = 0.0f;
		this->update_visual();
		this->on_value_changed(this->value);
		});

	//reposition
	this->reposition(this->position, this->size);
}

void ui_system::ui_slider::reposition(const glm::vec3& position_, const glm::vec2& size_)
{
	this->ui_button::reposition(position_, size_);
	this->background.anchor_point = position_,
	this->background.model_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(this->size, 1.0f));
	this->update_visual();
}

void ui_system::ui_slider::update_visual()
{
	//glm::vec2 scaled_size = this->scaled_size();
	this->fill.anchor_point = position;
	this->fill.model_matrix = glm::scale(glm::translate(glm::mat4(1.0f),
		glm::vec3(-this->size.x, 0.0f, 0.001f)),
		glm::vec3(this->value * this->size.x, this->size.y, 1.0f));
	this->handle.anchor_point = position;
	this->handle.model_matrix = glm::scale(glm::translate(glm::mat4(1.0f),
		glm::vec3(this->size.x * (this->value * 2.0f - 1.0f), 0.0f, 0.002f)),
		glm::vec3(this->size.y, this->size.y, 1.0f));
}
