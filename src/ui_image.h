#pragma once
#include "ui_visual.h"
namespace ui_system {
	class ui_image : public ui_system::ui_visual {
	public:
		renderer::texture_ptr image;
		ui_image(
			const std::string& image,
			const glm::vec3& anchor_point_ = glm::vec3(0.0f),
			const glm::mat4& model_matrix_ = glm::mat4(1.0f),
			const glm::vec3& pivot_point_ = glm::vec3(0.0f)
		);
		void draw() override;
	};
}

