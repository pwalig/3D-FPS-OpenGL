#include "ui_visual.h"
#include <lodepng.h>
#include "ui_system.h"
#include <glm/gtc/type_ptr.hpp>
#include "shaderprogram.h"


// TEXTURE

GLuint readTexture(const char* filename) {
	GLuint tex;
	glActiveTexture(GL_TEXTURE0);

	//Read into computers memory
	std::vector<unsigned char> image;   //Allocate memory 
	unsigned width, height;   //Variables for image size
	//Read the image
	unsigned error = lodepng::decode(image, width, height, filename);

	//Import to graphics card memory
	glGenTextures(1, &tex); //Initialize one handle
	glBindTexture(GL_TEXTURE_2D, tex); //Activate handle
	//Copy image to graphics cards memory reprezented by the active handle
	glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*)image.data());

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return tex;
}

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