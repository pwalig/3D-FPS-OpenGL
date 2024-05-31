#include "constant_model.h"
#include <renderer.h>
#include "shaderprogram.h"
#include <glm/gtc/type_ptr.hpp>

renderer::constant_model::constant_model(const std::string& mesh_, const glm::vec4& color_, const glm::mat4& initialMatrix)
	: model(mesh_, initialMatrix), color(color_) {}

void renderer::constant_model::draw()
{
	spConstant->use();//Aktywacja programu cieniuj¹cego
	glUniformMatrix4fv(spConstant->u("P"), 1, false, glm::value_ptr(renderer::active_camera.get_P()));
	glUniformMatrix4fv(spConstant->u("V"), 1, false, glm::value_ptr(renderer::active_camera.get_V()));
	glUniformMatrix4fv(spConstant->u("M"), 1, false, glm::value_ptr(this->model_matrix));
	glUniform4fv(spConstant->u("color"), 1, glm::value_ptr(color));

	glEnableVertexAttribArray(spConstant->a("vertex"));  //W³¹cz przesy³anie danych do atrybutu vertex
	glVertexAttribPointer(spConstant->a("vertex"), 4, GL_FLOAT, false, 0, this->mesh->vertices.data()); //Wska¿ tablicê z danymi dla atrybutu vertex

	//glDrawElements(GL_TRIANGLES, this->mesh->indices.size(), GL_INT, this->mesh->indices.data());
	glDrawArrays(GL_TRIANGLES, 0, this->mesh->vertices.size() / 4);

	glDisableVertexAttribArray(spConstant->a("vertex"));  //Wy³¹cz przesy³anie danych do atrybutu vertex
}
