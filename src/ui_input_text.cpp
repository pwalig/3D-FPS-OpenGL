#include "ui_input_text.h"

ui_system::ui_input_text::ui_input_text(const glm::vec3& position_, const glm::vec2& size_, const std::string& image, const std::string& text, const std::string& font, const glm::vec4& base_color_, const glm::vec4& hover_color_, const glm::vec4& hold_color_) :
	ui_text_button(position_, size_, image, text, font, base_color_, hover_color_, hold_color_)
{
	this->on_click.subscribe([this]() {
		if (input_system::active_text_field != &(this->text)) {
			input_system::active_text_field = &(this->text);
		}
		});

	this->reposition(position_, size_);
}
