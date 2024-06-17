#pragma once
#include <ui_vbutton.h>
#include <ui_image.h>

namespace ui_system {
	class ui_slider : public ui_vbutton {
	public:
		ui_system::ui_image handle;
		ui_system::ui_image background;
		ui_system::ui_image fill;

		glm::vec4 handle_base_color;
		glm::vec4 handle_hover_color;
		glm::vec4 handle_hold_color;

		glm::vec4 background_base_color;
		glm::vec4 background_hover_color;
		glm::vec4 background_hold_color;

		glm::vec4 fill_base_color;
		glm::vec4 fill_hover_color;
		glm::vec4 fill_hold_color;

		float value = 0.0f;
		std::function<void(float)> on_value_changed = [](float) {};

		ui_slider(const glm::vec3& position_ = glm::vec3(0.0f),
			const glm::vec2& size_ = glm::vec2(1.0f), 
			const glm::vec4& handle_color = glm::vec4(1.0f),
			const glm::vec4& background_color = glm::vec4(0.3f, 0.3f, 0.3f, 1.0f),
			const glm::vec4& fill_color = glm::vec4(0.7f, 0.7f, 0.7f, 1.0f)
		);

		void reposition(const glm::vec3& position_, const glm::vec2& size_ = glm::vec2(1.0f)) override;
		void update_visual();
	};
}
