#pragma once
#include "ui_text_button.h"

namespace ui_system{
	class ui_dropdown : public ui_text_button {
	public:
		std::vector<std::string> values;
		std::string value;
		std::vector<ui_text_button*> buttons;

		ui_dropdown(
			const glm::vec3& position_ = glm::vec3(0.0f),
			const glm::vec2& size_ = glm::vec2(1.0f),
			const std::string& image = "../assets/textures/White_Square.png",
			const std::string& text = "",
			const std::string& font = "../assets/fonts/bitmap/handwiriting-readable.png",
			const glm::vec4& base_color_ = glm::vec4(1.0f),
			const glm::vec4& hover_color_ = glm::vec4(0.85f, 0.85f, 0.85f, 1.0f),
			const glm::vec4& hold_color_ = glm::vec4(0.7f, 0.7f, 0.7f, 1.0f)
		);

		std::function<void(std::string)> on_value_changed = [](std::string) {};

		void reposition(const glm::vec3& position_, const glm::vec2& size_ = glm::vec2(0.0f)) override;
		void close();
		~ui_dropdown();
	};
}
