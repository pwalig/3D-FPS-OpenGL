#include "ui_button.h"

engine::object_subscription_list<ui_system::ui_button> ui_system::ui_button::all;
engine::object_subscription_list<ui_system::ui_button> ui_system::ui_button::mouse_hovered;

ui_system::ui_button::ui_button(const glm::vec2& position_, const glm::vec2& size_) : position(position_), size(size_), held(false)
{
	ui_system::ui_button::all.subscribe(this);
}

bool ui_system::ui_button::check_collision(const glm::vec2& mouse_pos)
{
	glm::vec2 lb = this->position - (this->size / 2.0f);
	glm::vec2 ub = this->position + (this->size / 2.0f);
	return mouse_pos.x >= lb.x && mouse_pos.x <= ub.x && mouse_pos.y >= lb.y && mouse_pos.y <= ub.y;
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
			if (uib->held) {
				uib->on_release.call_events(); // not sure if hovering mouse off the button while holding LMB should count as release
				uib->held = false;
			}
			ui_system::ui_button::mouse_hovered.unsubscribe(uib);
			uib->on_mouse_exit.call_events();
		}
		});
}
