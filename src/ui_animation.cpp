#include "ui_animation.h"
#include <ui_system.h>
#include "shaderprogram.h"
#include <time_system.h>

ui_system::ui_animation::ui_animation(const std::string& image_, const int& rows_, const int& columns_, const glm::vec3& anchor_point_, const glm::mat4& model_matrix_, const glm::vec3& pivot_point_) :
	ui_image(image_, anchor_point_, model_matrix_, pivot_point_), rows(rows_), columns(columns_),
	extent(glm::vec2(1.0f / (float)columns_, 1.0f / (float)rows_)),
	t(std::bind(&ui_system::ui_animation::loop_play, this))
{}

void ui_system::ui_animation::draw()
{
	glVertexAttribPointer(spUI->a("vertex"), 4, GL_FLOAT, false, 0, ui_system::quad::vertices);

	int progress;
	if (reverse) {
		progress = t.time / interval;
	} else {
		float total = this->interval * (float)rows * (float)columns;
		progress = (total - t.time) / interval;
	}

	glm::vec2 pos = glm::vec2(
			extent.x * (float)(progress % columns),
			extent.y * (float)(progress / columns));

	float texture_coordinates[12] = {
		pos.x, pos.y + extent.y,
		pos.x + extent.x, pos.y + extent.x,
		pos.x, pos.y,

		pos.x + extent.x, pos.y,
		pos.x + extent.x, pos.y + extent.y,
		pos.x, pos.y
	};

	glVertexAttribPointer(spUI->a("texCoord"), 2, GL_FLOAT, false, 0, texture_coordinates);

	glBindTexture(GL_TEXTURE_2D, this->image.get());
	glUniform1i(spUI->u("tex"), 0);

	glDrawArrays(GL_TRIANGLES, 0, ui_system::quad::vertex_count);
}

void ui_system::ui_animation::play(const float& interval_)
{
	this->interval = interval_;
	t.pause = false;
	t.start(this->interval * (float)rows * (float)columns);
}

void ui_system::ui_animation::pause()
{
	t.pause = true;
}

void ui_system::ui_animation::unpause()
{
	t.pause = false;
}

// stops animation, switches image to frame one (or first in row if stop_on_row_end == true)
void ui_system::ui_animation::stop()
{
	t.stop();
}

void ui_system::ui_animation::swap_image(const std::string& image_, const int& rows_, const int& columns_)
{
	stop();
	this->image = renderer::texture_ptr(image_);
	this->rows = rows_;
	this->columns = columns_;
	this->extent = glm::vec2(1.0f / (float)columns_, 1.0f / (float)rows_);
}

void ui_system::ui_animation::loop_play()
{
	if (loop) {
		play(this->interval * (float)rows * (float)columns);
	}
}
