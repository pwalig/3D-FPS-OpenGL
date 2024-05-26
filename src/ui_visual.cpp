#include "ui_visual.h"
#include <lodepng.h>
#include "ui_system.h"
#include <glm/gtc/type_ptr.hpp>
#include "shaderprogram.h"
#include <read_texture.h>


// UI VISUAL

std::vector<ui_system::ui_visual*> ui_system::ui_visual::all_ui_visuals;

ui_system::ui_visual::ui_visual(const char* filename, const glm::mat4& model_matrix_) : model_matrix(model_matrix_), tex(readTexture(filename))
{
	ui_system::ui_visual::all_ui_visuals.push_back(this);
}

void ui_system::ui_visual::draw() {}

void ui_system::ui_visual::swap_texture(const char* filename)
{
	glDeleteTextures(1, &tex);
	tex = readTexture(filename);
}

ui_system::ui_visual::~ui_visual()
{
	glDeleteTextures(1, &tex);

	std::vector<ui_system::ui_visual*>::iterator id = std::find(ui_system::ui_visual::all_ui_visuals.begin(), ui_system::ui_visual::all_ui_visuals.end(), this);
	if (id != ui_system::ui_visual::all_ui_visuals.end()) ui_system::ui_visual::all_ui_visuals.erase(id);
}

void ui_system::ui_visual::draw_ui()
{
	for (ui_system::ui_visual* uiv : ui_system::ui_visual::all_ui_visuals) {
		uiv->draw();
	}
}