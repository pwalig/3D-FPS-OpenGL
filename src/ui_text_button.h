#pragma once
#include "ui_vbutton.h"
namespace ui_system {
    class ui_text_button : public ui_vbutton {
    public:
		ui_image image;
        ui_text text;

		glm::vec4 base_color;
		glm::vec4 hover_color;
		glm::vec4 hold_color;

		ui_text_button(
			const glm::vec3& position_ = glm::vec3(0.0f),
			const glm::vec2& size_ = glm::vec2(1.0f),
			const std::string& image = "../assets/textures/White_Square.png",
			const std::string& text = "",
			const std::string& font = "../assets/fonts/bitmap/handwiriting-readable.png",
			const glm::vec4& base_color_ = glm::vec4(1.0f),
			const glm::vec4& hover_color_ = glm::vec4(0.85f, 0.85f, 0.85f, 1.0f),
			const glm::vec4& hold_color_ = glm::vec4(0.7f, 0.7f, 0.7f, 1.0f)
		);

		void reposition(const glm::vec3& position_, const glm::vec2& size_ = glm::vec2(0.0f)) override;
    };
}
