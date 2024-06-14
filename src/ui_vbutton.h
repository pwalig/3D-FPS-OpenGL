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

		ui_vbutton(const std::string& image, const std::string& text = "", const std::string& font = "../assets/fonts/bitmap/handwiriting-readable.png", const glm::vec4& color = glm::vec4(1.0f));
	};
}
