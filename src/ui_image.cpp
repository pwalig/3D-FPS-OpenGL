#include "ui_image.h"
#include <glm/gtc/type_ptr.hpp>
#include "ui_system.h"
#include "shaderprogram.h"

ui_system::ui_image::ui_image(const std::string& image, const glm::mat4& model_matrix_) :ui_visual(image, model_matrix_) {}

void ui_system::ui_image::draw()
{
	glUniformMatrix4fv(spUI->u("M"), 1, false, glm::value_ptr(this->model_matrix));
	glUniform4fv(spUI->u("color"), 1, glm::value_ptr(this->color));

	glVertexAttribPointer(spUI->a("vertex"), 4, GL_FLOAT, false, 0, ui_system::quad::vertices);
	glVertexAttribPointer(spUI->a("texCoord"), 2, GL_FLOAT, false, 0, ui_system::quad::texture_coordinates);

	glBindTexture(GL_TEXTURE_2D, *(this->texture.get()));
	glUniform1i(spUI->u("tex"), 0);

	glDrawArrays(GL_TRIANGLES, 0, ui_system::quad::vertex_count);
}
