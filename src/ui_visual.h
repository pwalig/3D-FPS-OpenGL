#pragma once
#include "ui_element.h"
#include <GL/glew.h>
#include <vector>

namespace ui_system {
	class ui_visual : public ui_system::ui_element {
	public:
		static std::vector<ui_visual*> all_ui_visuals;

		GLuint tex;
		int const* vertex_count;
		float const* vertices;
		float const* texture_coordinates;
		ui_visual(const glm::mat4& model_matrix_ = glm::mat4(1.0f));
		ui_visual(const char* filename, const glm::mat4& model_matrix_ = glm::mat4(1.0f));
		void draw() override;
		~ui_visual();

		static void draw_ui();
	};
}

