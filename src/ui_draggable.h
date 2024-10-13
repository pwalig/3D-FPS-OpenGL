#pragma once
#include "ui_button.h"
#include <ui_visual.h>
namespace ui_system {
	class ui_draggable : public ui_system::ui_button {
	public:
		ui_draggable(
			const glm::vec3& position_ = glm::vec3(0.0f),
			const glm::vec2& size_ = glm::vec2(1.0f),
			ui_system::ui_visual* content_ = nullptr
		);
	private:
		ui_system::ui_visual* content;
		glm::vec2 grab_offset;

		void update_position(const glm::vec2& position);
	};
}
