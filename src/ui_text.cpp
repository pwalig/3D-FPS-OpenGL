#include "ui_text.h"
#include <glm/gtc/type_ptr.hpp>
#include <ui_system.h>
#include "shaderprogram.h"

ui_system::ui_text::ui_text(const std::string& text_, const std::string& font_, const glm::vec3& anchor_point_, const glm::mat4& model_matrix_, const glm::vec3& pivot_point_) :
	text(text_), font(font_), width(1000.0f), ui_visual(anchor_point_, model_matrix_)
{
	this->ref_pivot = pivot_point_;
	this->pivot_point = this->pivot(pivot_point_);
}

ui_system::ui_text::ui_text(const ui_text& other) :
	ui_visual(other.anchor_point, other.model_matrix, other.pivot_point),
	text(other.text), font(other.font), width(other.width), ref_pivot(other.ref_pivot)
{
	this->pivot_point = this->pivot(this->pivot_point);
}

ui_system::ui_text::ui_text(ui_text&& other) noexcept :
	ui_visual(other.anchor_point, other.model_matrix, other.pivot_point),
	text(other.text), font(other.font), width(other.width), ref_pivot(other.ref_pivot)
{
	this->pivot_point = this->pivot(this->pivot_point);
}

void ui_system::ui_text::draw()
{
	glBindTexture(GL_TEXTURE_2D, this->font.get());
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

glm::vec3 ui_system::ui_text::pivot(const glm::vec3& pivot_point)
{
	// calculate extent
	glm::vec2 extent = glm::vec2(0.0f);
	float pos = 0.0f;
	if (!(this->text.empty())) extent.y += 0.5;
	for (const char& c : this->text) {
		if (c == '\n') {
			pos = 0.0f;
			extent.y += 0.5f;
		}
		else {
			pos += 1.0f;
			if (pos > extent.x) extent.x = pos;
			if (pos >= width) {
				pos = 0.0f;
				extent.y += 0.5f;
			}
		}
	}
	extent.x /= 2.0f;
	return glm::vec3(
		pivot_point.x * extent.x + extent.x,
		-pivot_point.y * extent.y - extent.y,
		0.0f
	);
}

void ui_system::ui_text::update_text(const std::string& new_text)
{
	this->text = new_text;
	this->update_pivot();
}

void ui_system::ui_text::update_pivot(const glm::vec3& new_pivot_point)
{
	this->ref_pivot = new_pivot_point;
	this->pivot_point = this->pivot(new_pivot_point);
}

void ui_system::ui_text::update_pivot()
{
	this->pivot_point = this->pivot(this->ref_pivot);
}
