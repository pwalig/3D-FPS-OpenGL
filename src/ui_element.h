#pragma once
#include <glm/glm.hpp>

namespace ui_system {
	class ui_element {
	public:
		glm::mat4 model_matrix;

		ui_element(const glm::mat4& model_matrix_ = glm::mat4(1.0f));
		virtual void draw();
	};
}

