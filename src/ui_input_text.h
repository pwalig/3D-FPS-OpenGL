#pragma once
#include "input_system.h"
#include "ui_text_button.h"
#include <key_bind.h>

namespace ui_system {
	class ui_input_text : public ui_text_button {
	public:
		ui_input_text(
			const glm::vec3& position_ = glm::vec3(0.0f),
			const glm::vec2& size_ = glm::vec2(1.0f),
			const std::string& image = "../assets/textures/White_Square.png",
			const std::string& text = "",
			const std::string& font = "../assets/fonts/bitmap/handwiriting-readable.png",
			const glm::vec4& base_color_ = glm::vec4(1.0f),
			const glm::vec4& hover_color_ = glm::vec4(0.85f, 0.85f, 0.85f, 1.0f),
			const glm::vec4& hold_color_ = glm::vec4(0.7f, 0.7f, 0.7f, 1.0f)
		);

		input_system::key_bind leave1_key_bind = input_system::key_bind([this]() {
			if (input_system::active_text_field == &(this->text)) {
				input_system::active_text_field = nullptr;
			}
			}, GLFW_KEY_ENTER, GLFW_PRESS);
		input_system::key_bind leave2_key_bind = input_system::key_bind([this]() {
			if (input_system::active_text_field == &(this->text)) {
				input_system::active_text_field = nullptr;
			}
			}, GLFW_KEY_ESCAPE, GLFW_PRESS);
		input_system::key_bind leave3_key_bind = input_system::key_bind([this]() {
			if (input_system::active_text_field == &(this->text)) {
				input_system::active_text_field = nullptr;
			}
			}, GLFW_MOUSE_BUTTON_1, GLFW_PRESS);
	};
}
