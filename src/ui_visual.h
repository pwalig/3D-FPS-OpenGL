#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>
#include "textures.h"

namespace ui_system {
	class ui_visual {
	public:
		static std::vector<ui_visual*> all_ui_visuals;

		glm::mat4 model_matrix;
		glm::vec3 pivot_point;
		glm::vec3 anchor_point;
		glm::vec4 color = glm::vec4(1.0f);
		ui_visual(
			const glm::vec3& anchor_point_ = glm::vec3(0.0f),
			const glm::mat4& model_matrix_ = glm::mat4(1.0f),
			const glm::vec3& pivot_point_ = glm::vec3(0.0f),
			const glm::vec4 color_ = glm::vec4(1.0f)
		);
		ui_visual(const ui_visual& other);
		ui_visual(ui_visual&& other) noexcept;
		virtual void draw();
		virtual ~ui_visual();

		static void draw_ui();
	};
}

