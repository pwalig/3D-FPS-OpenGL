#pragma once
#include "event_subscription_list.h"
#include "object_subscription_list.h"
#include <glm/glm.hpp>

namespace ui_system {
	class ui_button {
	public:
		glm::vec2 size;
		glm::vec3 position;
		bool held;
		bool ignore_overlaying;
		bool transparent;
		ui_button(const glm::vec3& position_ = glm::vec3(0.0f), const glm::vec2& size_ = glm::vec2(1.0f));

		engine::event_subscription_list<glm::vec2> on_mouse_enter, on_mouse_stay, on_drag;
		engine::event_subscription_list<> on_mouse_exit, on_click, on_release;

		bool check_collision(const glm::vec2& mouse_pos);
		glm::vec2 scaled_size();
		virtual void reposition(const glm::vec3& position_, const glm::vec2& size_ = glm::vec2(0.0f));

		virtual ~ui_button();

		static engine::object_subscription_list<ui_system::ui_button> all;
		static engine::object_subscription_list<ui_system::ui_button> mouse_hovered;
		static void check_for_mouse_collisions(const glm::vec2& mouse_pos);
	};
}

