#include "ui_button.h"
#include "ui_system.h"
#include <engine.h>

engine::object_subscription_list<ui_system::ui_button> ui_system::ui_button::all;
engine::object_subscription_list<ui_system::ui_button> ui_system::ui_button::mouse_hovered;

ui_system::ui_button::ui_button(const glm::vec3& position_, const glm::vec2& size_) : position(position_), size(size_), held(false)
{
	ui_system::ui_button::all.subscribe(this);
}

bool ui_system::ui_button::check_collision(const glm::vec2& mouse_pos)
{
	glm::vec2 ssize = this->scaled_size();
	glm::vec2 lb = glm::vec2(this->position) - ssize;
	glm::vec2 ub = glm::vec2(this->position) + ssize;
	return mouse_pos.x >= lb.x && mouse_pos.x <= ub.x && mouse_pos.y >= lb.y && mouse_pos.y <= ub.y;
}

glm::vec2 ui_system::ui_button::scaled_size()
{
	float aspect_ratio = engine::height / engine::width;
	return glm::vec2(
		this->size.x * ((1.0f - ui_system::scaling) * aspect_ratio + ui_system::scaling),
		this->size.y * (ui_system::scaling * aspect_ratio + (1.0f - ui_system::scaling))
	);
}

void ui_system::ui_button::reposition(const glm::vec3& position_, const glm::vec2& size_)
{
	this->position = position_;
	if (glm::dot(size_, size_) >= 0.0f) {
		this->size = size_;
	}
}

ui_system::ui_button::~ui_button()
{
	ui_system::ui_button::all.unsubscribe(this);
	ui_system::ui_button::mouse_hovered.unsubscribe(this);
}

void ui_system::ui_button::check_for_mouse_collisions(const glm::vec2& mouse_pos)
{
	ui_system::ui_button::all.perform_on_all([&mouse_pos](ui_system::ui_button* uib) { // for each ui button
		if (uib->check_collision(mouse_pos)) { // check if mouse hovers over it
			if (ui_system::ui_button::mouse_hovered.on_list(uib)) uib->on_mouse_stay.call_events(mouse_pos);
			else {
				uib->on_mouse_enter.call_events(mouse_pos);
				ui_system::ui_button::mouse_hovered.subscribe(uib);
			}
		}
		else if (ui_system::ui_button::mouse_hovered.on_list(uib)) {
			if (!(uib->held)) {
				ui_system::ui_button::mouse_hovered.unsubscribe(uib);
				uib->on_mouse_exit.call_events();
			}
		}
		if (uib->held) uib->on_drag.call_events(mouse_pos);
		});
}
