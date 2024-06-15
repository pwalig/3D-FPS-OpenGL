#pragma once
#include <ui_vbutton.h>

namespace ui_system {
	class ui_slider {
	public:
		ui_system::ui_vbutton handle;
		ui_system::ui_image background;
		ui_system::ui_image fill;

		float value = 0.0f;
		glm::vec3 position;
		glm::vec2 size;

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
