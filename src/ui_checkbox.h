#pragma once
#include "ui_button.h"
#include <ui_image.h>

namespace ui_system {
	class ui_checkbox : public ui_button {
	public:
		ui_image background;
		ui_image* check;
		std::string check_image;

		glm::vec4 background_base_color;
		glm::vec4 background_hover_color;
		glm::vec4 background_hold_color;

		glm::vec4 check_base_color;
		glm::vec4 check_hover_color;
		glm::vec4 check_hold_color;

		bool get_value();

		ui_checkbox(
			const glm::vec3& position_ = glm::vec3(0.0f),
			const glm::vec2& size_ = glm::vec2(1.0f),
			const std::string& image_ = "../assets/textures/White_Square.png",
			const std::string& check_image_ = "../assets/textures/White_Square.png"
		);

		void reposition(const glm::vec3& position_, const glm::vec2& size_ = glm::vec2(0.0f)) override;
		void update_check(const bool& value);

		~ui_checkbox();

	private:
		glm::mat4 check_model_matrix();
		glm::vec3 check_anchor_point();
	};
}
