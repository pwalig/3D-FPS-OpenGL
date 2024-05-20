#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>

namespace ui_system {
	class ui_visual {
	public:
		static std::vector<ui_visual*> all_ui_visuals;

		glm::mat4 model_matrix;
		GLuint tex;
		int const* vertex_count;
		float const* vertices;
		float const* texture_coordinates;
		ui_visual(const glm::mat4& model_matrix_ = glm::mat4(1.0f));
		ui_visual(const char* filename, const glm::mat4& model_matrix_ = glm::mat4(1.0f));
		void draw();
		void swap_texture(const char* filename);
		~ui_visual();

		static void draw_ui();
	};
}

