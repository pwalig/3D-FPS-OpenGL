#pragma once
#include "ui_button.h"
#include <ui_text.h>
#include <ui_image.h>
#include <textures.h>

namespace ui_system {
	class ui_vbutton : public ui_button {
	public:
		ui_image image;
		ui_text text;

		renderer::texture_ptr base_image;
		renderer::texture_ptr hover_image;
		renderer::texture_ptr hold_image;

		glm::vec4 base_color;
		glm::vec4 hover_color;
		glm::vec4 hold_color;

		ui_vbutton(
			const glm::vec3& position_ = glm::vec3(0.0f),
			const glm::vec2& size_ = glm::vec2(1.0f),
			const glm::vec4& color = glm::vec4(1.0f),
			const std::string& image = "../assets/textures/White_Square.png",
			const std::string& text = "",
			const std::string& font = "../assets/fonts/bitmap/handwiriting-readable.png"
		);

		void reposition(const glm::vec3& position_, glm::vec2 size_ = glm::vec2(0.0f));
	};
}
