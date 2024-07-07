#include "pbr_model.h"
#include "shaderprogram.h"
#include <glm/gtc/type_ptr.hpp>
#include <renderer.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include "player_script.h"
#include <light.h>

renderer::pbr_model::pbr_model(const std::string& mesh_, const std::string& normal_map, const std::string& diffuse_map, const std::string& height_map, const glm::mat4& initialMatrix) :
	model(mesh_, initialMatrix), diffuse(renderer::get_texture(diffuse_map)), normal(renderer::get_texture(normal_map)), height(renderer::get_texture(height_map)) {}

void renderer::pbr_model::draw()
{
	spPBR->use();//Aktywacja programu cieniuj¹cego
	//Przeslij parametry programu cieniuj¹cego do karty graficznej
	glUniformMatrix4fv(spPBR->u("P"), 1, false, glm::value_ptr(renderer::active_camera.get_P()));
	glUniformMatrix4fv(spPBR->u("V"), 1, false, glm::value_ptr(renderer::active_camera.get_V()));
	glUniformMatrix4fv(spPBR->u("M"), 1, false, glm::value_ptr(this->model_matrix));

	//light data
	glUniform1i(spPBR->u("lights"), renderer::light::all_lights.size()); // how many lights
	glUniform3fv(spPBR->u("light_positions"), renderer::light::all_lights.size(), renderer::light::get_light_positions().data());
	glUniform3fv(spPBR->u("light_colors"), renderer::light::all_lights.size(), renderer::light::get_light_colors().data());

	glEnableVertexAttribArray(spPBR->a("vertex"));  //W³¹cz przesy³anie danych do atrybutu vertex
	glVertexAttribPointer(spPBR->a("vertex"), 4, GL_FLOAT, false, 0, this->mesh->vertices.data()); //Wska¿ tablicê z danymi dla atrybutu vertex

	glEnableVertexAttribArray(spPBR->a("texture_coordinates"));  //W³¹cz przesy³anie danych do atrybutu texCoord
	glVertexAttribPointer(spPBR->a("texture_coordinates"), 2, GL_FLOAT, false, 0, this->mesh->texCoords.data()); //Wska¿ tablicê z danymi dla atrybutu texCoord

	glEnableVertexAttribArray(spPBR->a("c1"));  //W³¹cz przesy³anie danych do atrybutu vertex
	glVertexAttribPointer(spPBR->a("c1"), 4, GL_FLOAT, false, 0, this->mesh->c1.data()); //Wska¿ tablicê z danymi dla atrybutu vertex
	glEnableVertexAttribArray(spPBR->a("c2"));  //W³¹cz przesy³anie danych do atrybutu vertex
	glVertexAttribPointer(spPBR->a("c2"), 4, GL_FLOAT, false, 0, this->mesh->c2.data()); //Wska¿ tablicê z danymi dla atrybutu vertex
	glEnableVertexAttribArray(spPBR->a("c3"));  //W³¹cz przesy³anie danych do atrybutu vertex
	glVertexAttribPointer(spPBR->a("c3"), 4, GL_FLOAT, false, 0, this->mesh->c3.data()); //Wska¿ tablicê z danymi dla atrybutu vertex

	glUniform1i(spPBR->u("diffuse_map"), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, *(this->diffuse));

	glUniform1i(spPBR->u("normal_map"), 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, *(this->normal));

	glUniform1i(spPBR->u("height_map"), 2);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, *(this->height));
	
	if (mesh->indices.empty()) glDrawArrays(GL_TRIANGLES, 0, this->mesh->vertices.size() / 4);
	else glDrawElements(GL_TRIANGLES, this->mesh->indices.size(), GL_UNSIGNED_INT, this->mesh->indices.data());

	glDisableVertexAttribArray(spPBR->a("vertex"));  //Wy³¹cz przesy³anie danych do atrybutu vertex
	glDisableVertexAttribArray(spPBR->a("c1"));  //Wy³¹cz przesy³anie danych do atrybutu vertex
	glDisableVertexAttribArray(spPBR->a("c2"));  //Wy³¹cz przesy³anie danych do atrybutu vertex
	glDisableVertexAttribArray(spPBR->a("c3"));  //Wy³¹cz przesy³anie danych do atrybutu vertex
	glDisableVertexAttribArray(spPBR->a("texture_coordinates"));  //Wy³¹cz przesy³anie danych do atrybutu texCoord0
}
