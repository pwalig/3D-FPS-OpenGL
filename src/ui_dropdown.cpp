#include "ui_dropdown.h"
#include <scripts_system.h>

ui_system::ui_dropdown::ui_dropdown(const glm::vec3& position_, const glm::vec2& size_, const std::string& image, const std::string& text, const std::string& font, const glm::vec4& base_color_, const glm::vec4& hover_color_, const glm::vec4& hold_color_) :
	ui_text_button(position_, size_, image, text, font, base_color_, hover_color_, hold_color_)
{
	this->on_click.subscribe([this, image, font]() {
		if (this->buttons.empty()) {
			float dist = 0.0f;
			for (const std::string& str : this->values) {
				dist -= 2.0f;
				this->buttons.push_back(new ui_text_button(
					this->position + glm::vec3(0.0f, this->size.y * dist, 0.0f),
					this->size, image, str, font,
					this->base_color, this->hover_color, this->hold_color
				));
				this->buttons.back()->on_click.subscribe([this, str]() {
					this->update_text(str);
					this->on_value_changed(str);
					scripts_system::events[SCRIPTS_START].subscribe([this]() {this->close(); }); // delay this action bc it deletes the button that is callin it
					});
			}
		}
		else this->close();
		});

	this->reposition(position_, size_);
}

void ui_system::ui_dropdown::reposition(const glm::vec3& position_, const glm::vec2& size_)
{
	this->ui_text_button::reposition(position_, size_);
	float dist = 0.0f;
	for (ui_text_button* uit : this->buttons) {
		dist -= 2.0f;
		uit->reposition(position_ + glm::vec3(0.0f, this->size.y * dist, 0.0f), this->size);
	}
}

void ui_system::ui_dropdown::close()
{
	for (ui_text_button* uit : this->buttons) delete uit;
	this->buttons.clear();
}

ui_system::ui_dropdown::~ui_dropdown()
{
	this->close();
}
