#include "ui_vbutton.h"

ui_system::ui_vbutton::ui_vbutton(const std::string& image, const std::string& text_, const std::string& font, const glm::vec4& color) :
	base_image(renderer::get_texture(image)), hover_image(renderer::get_texture(image)), hold_image(renderer::get_texture(image)),
	text(text_, font.c_str()), base_color(color), hover_color(color), hold_color(color), image(image.c_str())
{
	/*this->on_mouse_enter.subscribe([this](glm::vec2 mp) {
		this->image.swap_texture()
		})*/
}
