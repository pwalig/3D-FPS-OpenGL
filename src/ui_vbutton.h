#pragma once
#include "ui_button.h"
#include <ui_text.h>
#include <ui_image.h>
#include <textures.h>

namespace ui_system {
	class ui_vbutton : public ui_button {
	public:
		float depth;
		glm::vec3 position3();

		ui_vbutton(
			const glm::vec3& position_ = glm::vec3(0.0f),
			const glm::vec2& size_ = glm::vec2(1.0f)
		);

		virtual void reposition(const glm::vec3& position_, const glm::vec2& size_ = glm::vec2(0.0f));
	};
}
