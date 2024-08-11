#include "ui_model.h"
#include "shaderprogram.h"

ui_system::ui_model::ui_model(const std::string& mesh_, const std::string& texture_, const glm::vec3& anchor_point_, const glm::mat4& model_matrix_, const glm::vec3& pivot_point_) :
	ui_visual(anchor_point_, model_matrix_, pivot_point_), mesh(renderer::mesh::get_mesh(mesh_)), texture(renderer::get_texture(texture_)) {}

ui_system::ui_model::ui_model(const ui_model& other) :
	ui_visual(other.anchor_point, other.model_matrix, other.pivot_point), mesh(other.mesh), texture(other.texture) {}

ui_system::ui_model::ui_model(ui_model&& other) :
	ui_visual(other.anchor_point, other.model_matrix, other.pivot_point), mesh(other.mesh), texture(other.texture) {}

void ui_system::ui_model::draw()
{
	//printf("draw\n");
	glVertexAttribPointer(spUI->a("vertex"), 4, GL_FLOAT, false, 0, this->mesh->vertices.data());
	glVertexAttribPointer(spUI->a("texCoord"), 2, GL_FLOAT, false, 0, this->mesh->texCoords.data());

	glBindTexture(GL_TEXTURE_2D, *(this->texture.get()));
	glUniform1i(spUI->u("texCoord"), 0);

	if (mesh->indices.empty()) glDrawArrays(GL_TRIANGLES, 0, this->mesh->vertices.size() / 4);
	else glDrawElements(GL_TRIANGLES, this->mesh->indices.size(), GL_UNSIGNED_INT, this->mesh->indices.data());
}
