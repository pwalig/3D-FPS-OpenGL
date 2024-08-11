#include "ui_image.h"
#include <glm/gtc/type_ptr.hpp>
#include "ui_system.h"
#include "shaderprogram.h"

ui_system::ui_image::ui_image(const std::string& image_, const glm::vec3& anchor_point_, const glm::mat4& model_matrix_, const glm::vec3& pivot_point_) :
	ui_visual(anchor_point_, model_matrix_, pivot_point_), image(renderer::get_texture(image_)) {}

ui_system::ui_image::ui_image(const ui_image& other) : 
	ui_visual(other.anchor_point, other.model_matrix, other.pivot_point), image(other.image) {}

ui_system::ui_image::ui_image(ui_image&& other) noexcept :
	ui_visual(other.anchor_point, other.model_matrix, other.pivot_point), image(other.image) {}

void ui_system::ui_image::draw()
{
	glVertexAttribPointer(spUI->a("vertex"), 4, GL_FLOAT, false, 0, ui_system::quad::vertices);
	glVertexAttribPointer(spUI->a("texCoord"), 2, GL_FLOAT, false, 0, ui_system::quad::texture_coordinates);

	glBindTexture(GL_TEXTURE_2D, *(this->image.get()));
	glUniform1i(spUI->u("tex"), 0);

	glDrawArrays(GL_TRIANGLES, 0, ui_system::quad::vertex_count);
}
