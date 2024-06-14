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
		renderer::texture_ptr texture;
		glm::vec4 color = glm::vec4(1.0f);
		ui_visual(const std::string& texture_, const glm::mat4& model_matrix_ = glm::mat4(1.0f));
		virtual void draw();
		void swap_texture(const std::string& new_texture);
		void swap_texture(const renderer::texture_ptr& new_texture);
		virtual ~ui_visual();

		static void draw_ui();
	};
}

