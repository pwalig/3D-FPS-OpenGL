#include "ui_visual.h"
#include <lodepng.h>
#include "ui_system.h"
#include <glm/gtc/type_ptr.hpp>
#include "shaderprogram.h"
#include <textures.h>


// UI VISUAL

std::vector<ui_system::ui_visual*> ui_system::ui_visual::all_ui_visuals;

ui_system::ui_visual::ui_visual(const std::string& texture_, const glm::mat4& model_matrix_) : model_matrix(model_matrix_), texture(renderer::get_texture(texture_))
{
	ui_system::ui_visual::all_ui_visuals.push_back(this);
}

void ui_system::ui_visual::draw() {}

void ui_system::ui_visual::swap_texture(const std::string& new_texture)
{
	texture = renderer::get_texture(new_texture);
}

void ui_system::ui_visual::swap_texture(const renderer::texture_ptr& new_texture)
{
	texture = new_texture;
}

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
		uiv->draw();
	}

	glDisableVertexAttribArray(spUI->a("vertex"));
	glDisableVertexAttribArray(spUI->a("texCoord"));
}