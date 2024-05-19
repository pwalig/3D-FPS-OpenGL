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

ui_system::ui_visual::ui_visual(const glm::mat4& model_matrix_) : ui_element(model_matrix_), tex(),
	vertex_count(&ui_system::quad::vertex_count), vertices(ui_system::quad::vertices), texture_coordinates(ui_system::quad::texture_coordinates) // quad is default
{
	ui_system::ui_visual::all_ui_visuals.push_back(this);
}

ui_system::ui_visual::ui_visual(const char* filename, const glm::mat4& model_matrix_) : ui_element(model_matrix_), tex(readTexture(filename)),
	vertex_count(&ui_system::quad::vertex_count), vertices(ui_system::quad::vertices), texture_coordinates(ui_system::quad::texture_coordinates) // quad is default
{
	ui_system::ui_visual::all_ui_visuals.push_back(this);
}

void ui_system::ui_visual::draw()
{
	spUI->use();

	glUniformMatrix4fv(spUI->u("P"), 1, false, glm::value_ptr(ui_system::P));
	glUniformMatrix4fv(spUI->u("M"), 1, false, glm::value_ptr(this->model_matrix));


	glEnableVertexAttribArray(spUI->a("vertex"));
	glVertexAttribPointer(spUI->a("vertex"), 4, GL_FLOAT, false, 0, this->vertices);

	glEnableVertexAttribArray(spUI->a("texCoord"));
	glVertexAttribPointer(spUI->a("texCoord"), 2, GL_FLOAT, false, 0, this->texture_coordinates);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, this->tex);
	glUniform1i(spUI->u("tex"), 0);

	glDrawArrays(GL_TRIANGLES, 0, *(this->vertex_count));

	glDisableVertexAttribArray(spUI->a("vertex"));
	glDisableVertexAttribArray(spUI->a("texCoord"));
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