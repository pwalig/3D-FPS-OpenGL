#include "ui_visual.h"
#include <lodepng.h>
#include "ui_system.h"
#include <glm/gtc/type_ptr.hpp>
#include "shaderprogram.h"
#include <textures.h>
#include <engine.h>


// UI VISUAL

std::vector<ui_system::ui_visual*> ui_system::ui_visual::all_ui_visuals;

ui_system::ui_visual::ui_visual(const glm::vec3& anchor_point_, const glm::mat4& model_matrix_, const glm::vec3& pivot_point_, const glm::vec4 color_) :
	anchor_point(anchor_point_), model_matrix(model_matrix_), pivot_point(pivot_point_), color(color_)
{
	ui_system::ui_visual::all_ui_visuals.push_back(this);
}

ui_system::ui_visual::ui_visual(const ui_visual& other) :
	ui_visual(other.anchor_point, other.model_matrix, other.pivot_point, other.color) {}

ui_system::ui_visual::ui_visual(ui_visual&& other) noexcept :
	anchor_point(glm::vec3(0.0f)), model_matrix(glm::mat4(1.0f)), pivot_point(glm::vec3(0.0f)), color(glm::vec4(0.0f))
{
	this->anchor_point = other.anchor_point;
	this->model_matrix = other.model_matrix;
	this->pivot_point = other.pivot_point;
	this->color = other.color;
	ui_system::ui_visual::all_ui_visuals.push_back(this);
}

void ui_system::ui_visual::draw() {}

ui_system::ui_visual::~ui_visual()
{
	std::vector<ui_system::ui_visual*>::iterator id = std::find(ui_system::ui_visual::all_ui_visuals.begin(), ui_system::ui_visual::all_ui_visuals.end(), this);
	if (id != ui_system::ui_visual::all_ui_visuals.end()) ui_system::ui_visual::all_ui_visuals.erase(id);
}

void ui_system::ui_visual::draw_ui()
{
	spUI->use();

	glUniformMatrix4fv(spUI->u("P"), 1, false, glm::value_ptr(ui_system::P));

	glEnableVertexAttribArray(spUI->a("vertex"));
	glEnableVertexAttribArray(spUI->a("texCoord"));

	glActiveTexture(GL_TEXTURE0);

	for (ui_system::ui_visual* uiv : ui_system::ui_visual::all_ui_visuals) {
		glUniformMatrix4fv(spUI->u("M"), 1, false, glm::value_ptr(uiv->model_matrix));
		glm::mat4 anchor = glm::translate(glm::mat4(1.0f), glm::vec3(
			uiv->anchor_point.x * engine::width,
			uiv->anchor_point.y * engine::height,
			-uiv->anchor_point.z));
		anchor = glm::scale(anchor, glm::vec3(
			(ui_system::scaling * engine::width) + ((1.0f - ui_system::scaling) * engine::height),
			(ui_system::scaling * engine::width) + ((1.0f - ui_system::scaling) * engine::height),
			1.0f
		));
		glUniformMatrix4fv(spUI->u("A"), 1, false, glm::value_ptr(anchor));
		glUniform3fv(spUI->u("pivot"), 1, glm::value_ptr(uiv->pivot_point));
		glUniform4fv(spUI->u("color"), 1, glm::value_ptr(uiv->color));
		uiv->draw();
	}

	glDisableVertexAttribArray(spUI->a("vertex"));
	glDisableVertexAttribArray(spUI->a("texCoord"));
}