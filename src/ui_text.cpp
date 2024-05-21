#include "ui_text.h"
#include <glm/gtc/type_ptr.hpp>
#include <ui_system.h>
#include "shaderprogram.h"

ui_system::ui_text::ui_text(const std::string& text_, const char* filename, const glm::mat4& model_matrix_) : ui_visual(filename, model_matrix_), text(text_), width(1000.0f)
{
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

void ui_system::ui_text::draw()
{
	spUI->use();

	glUniformMatrix4fv(spUI->u("P"), 1, false, glm::value_ptr(ui_system::P));
	glUniformMatrix4fv(spUI->u("M"), 1, false, glm::value_ptr(this->model_matrix));
	glUniform4fv(spUI->u("color"), 1, glm::value_ptr(this->color));

	glEnableVertexAttribArray(spUI->a("vertex"));
	glEnableVertexAttribArray(spUI->a("texCoord"));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, this->tex);
	glUniform1i(spUI->u("tex"), 0);

	glm::vec2 pos = glm::vec2(0.0f);
	for (const char& c : this->text) {
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
	glDisableVertexAttribArray(spUI->a("vertex"));
	glDisableVertexAttribArray(spUI->a("texCoord"));
}
