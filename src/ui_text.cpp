#include "ui_text.h"
#include <glm/gtc/type_ptr.hpp>
#include <ui_system.h>
#include "shaderprogram.h"

ui_system::ui_text::ui_text(const std::string& text_, const std::string& font_, const glm::vec3& anchor_point_, const glm::mat4& model_matrix_, const glm::vec3& pivot_point_) :
	ui_visual(anchor_point_, model_matrix_, pivot_point_), text(text_), font(renderer::get_texture(font_)), width(1000.0f) {}

void ui_system::ui_text::draw()
{
	glBindTexture(GL_TEXTURE_2D, *(this->font.get()));
	glUniform1i(spUI->u("tex"), 0);

	glm::vec2 pos = glm::vec2(0.0f);
	for (const char& c : this->text) {
		if (c == '\n') {
			pos.x = 0.0f;
			pos.y -= 1.0f;
			continue;
		}
		float vertices[24] = {
			pos.x, pos.y - 1.0f, 0.0f, 1.0f,
			pos.x + 1.0f, pos.y - 1.0f, 0.0f, 1.0f,
			pos.x, pos.y, 0.0f, 1.0f,

			pos.x + 1.0f, pos.y, 0.0f, 1.0f,
			pos.x + 1.0f, pos.y - 1.0f, 0.0f, 1.0f,
			pos.x, pos.y, 0.0f, 1.0f
		};
		int char_id = c - 32;
		glm::vec2 tc = glm::vec2((float)(char_id % 16) / 16.0f, (float)(char_id / 16) / 6.0f);
		float texture_coordinates[12] = {
			tc.x, tc.y + 0.1667f,
			tc.x + 0.0625f, tc.y + 0.1667f,
			tc.x, tc.y,

			tc.x + 0.0625f, tc.y,
			tc.x + 0.0625f, tc.y + 0.1667f,
			tc.x, tc.y
		};
		glVertexAttribPointer(spUI->a("vertex"), 4, GL_FLOAT, false, 0, vertices);

		glVertexAttribPointer(spUI->a("texCoord"), 2, GL_FLOAT, false, 0, texture_coordinates);

		glDrawArrays(GL_TRIANGLES, 0, 6);

		pos.x += 1.0f;
		if (pos.x >= width) {
			pos.x = 0.0f;
			pos.y -= 1.0f;
		}
	}
}
