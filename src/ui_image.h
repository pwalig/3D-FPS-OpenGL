#pragma once
#include "ui_visual.h"
namespace ui_system {
	class ui_image : public ui_system::ui_visual {
	public:
		ui_image(const std::string& image, const glm::mat4& model_matrix_ = glm::mat4(1.0f));
		void draw() override;
	};
}

