#pragma once
#include <ui_button.h>
#include <ui_image.h>

namespace ui_system {
	class ui_slider {
	public:
		ui_system::ui_button area;
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
		float depth;
		glm::vec3 position3();

		ui_slider(const glm::vec3& position_ = glm::vec3(0.0f),
			const glm::vec2& size_ = glm::vec2(1.0f), 
			const glm::vec4& handle_color = glm::vec4(1.0f),
			const glm::vec4& background_color = glm::vec4(0.4f, 0.4f, 0.4f, 1.0f),
			const glm::vec4& fill_color = glm::vec4(0.8f, 0.8f, 0.8f, 1.0f)
		);

		void reposition(const glm::vec3& position_ = glm::vec3(0.0f), glm::vec2 size_ = glm::vec2(1.0f));
		void update_visual();
	};
}
