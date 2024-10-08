#include "pbr_model.h"
#include "shaderprogram.h"
#include <glm/gtc/type_ptr.hpp>
#include <renderer.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include "player_script.h"
#include <light.h>

renderer::pbr_model::pbr_model(
	const std::string& mesh_,
	const std::string& normal_map,
	const std::string& albedo_map,
	const std::string& height_map,
	const std::string& data_map,
	const glm::mat4& initialMatrix
) :
	model(mesh_, initialMatrix), albedo(albedo_map), normal(normal_map), height(height_map), data(data_map)
{}

void renderer::pbr_model::draw()
{
	spPBR->use();//Aktywacja programu cieniuj�cego
	glUniform1f(spPBR->u("gamma"), renderer::gamma);
	glUniform1f(spPBR->u("exposure"), renderer::exposure);

	//Przeslij parametry programu cieniuj�cego do karty graficznej
	glUniformMatrix4fv(spPBR->u("P"), 1, false, glm::value_ptr(renderer::active_camera.get_P()));
	glUniformMatrix4fv(spPBR->u("V"), 1, false, glm::value_ptr(renderer::active_camera.get_V()));
	glUniformMatrix4fv(spPBR->u("M"), 1, false, glm::value_ptr(this->model_matrix));

	//light data
	glUniform1i(spPBR->u("lights"), renderer::light::all_lights.size()); // how many lights
	glUniform3fv(spPBR->u("light_positions"), renderer::light::all_lights.size(), renderer::light::get_light_positions().data());
	glUniform3fv(spPBR->u("light_colors"), renderer::light::all_lights.size(), renderer::light::get_light_colors().data());

	glEnableVertexAttribArray(spPBR->a("vertex"));  //W��cz przesy�anie danych do atrybutu vertex
	glVertexAttribPointer(spPBR->a("vertex"), 4, GL_FLOAT, false, 0, this->mesh->vertices.data()); //Wska� tablic� z danymi dla atrybutu vertex

	glEnableVertexAttribArray(spPBR->a("texture_coordinates"));  //W��cz przesy�anie danych do atrybutu texCoord
	glVertexAttribPointer(spPBR->a("texture_coordinates"), 2, GL_FLOAT, false, 0, this->mesh->texCoords.data()); //Wska� tablic� z danymi dla atrybutu texCoord

	glEnableVertexAttribArray(spPBR->a("normal"));  //W��cz przesy�anie danych do atrybutu vertex
	glVertexAttribPointer(spPBR->a("normal"), 3, GL_FLOAT, false, 0, this->mesh->normals.data()); //Wska� tablic� z danymi dla atrybutu vertex
	glEnableVertexAttribArray(spPBR->a("tangent"));  //W��cz przesy�anie danych do atrybutu vertex
	glVertexAttribPointer(spPBR->a("tangent"), 3, GL_FLOAT, false, 0, this->mesh->tangents.data()); //Wska� tablic� z danymi dla atrybutu vertex
	glEnableVertexAttribArray(spPBR->a("bitangent"));  //W��cz przesy�anie danych do atrybutu vertex
	glVertexAttribPointer(spPBR->a("bitangent"), 3, GL_FLOAT, false, 0, this->mesh->bitangents.data()); //Wska� tablic� z danymi dla atrybutu vertex

	glUniform1i(spPBR->u("albedo_map"), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, this->albedo.get());

	glUniform1i(spPBR->u("normal_map"), 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, this->normal.get());

	glUniform1i(spPBR->u("height_map"), 2);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, this->height.get());

	glUniform1i(spPBR->u("data_map"), 3);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, this->data.get());

	glUniform1i(spPBR->u("skybox"), 4);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_CUBE_MAP, renderer::global_cube_map);

	glUniform3fv(spPBR->u("albedo_"), 1, glm::value_ptr(this->albedo_));
	glUniform1f(spPBR->u("roughness_"), this->roughness_);
	glUniform1f(spPBR->u("metallic_"), this->metallic_);
	glUniform1f(spPBR->u("emission_"), this->emission_);
	glUniform1f(spPBR->u("ao_"), this->ao_);
	glUniform3fv(spPBR->u("ambient_"), 1, glm::value_ptr(this->ambient_));
	
	if (mesh->indices.empty()) glDrawArrays(GL_TRIANGLES, 0, this->mesh->vertices.size() / 4);
	else glDrawElements(GL_TRIANGLES, this->mesh->indices.size(), GL_UNSIGNED_INT, this->mesh->indices.data());

	glDisableVertexAttribArray(spPBR->a("vertex"));  //Wy��cz przesy�anie danych do atrybutu vertex
	glDisableVertexAttribArray(spPBR->a("normal"));  //Wy��cz przesy�anie danych do atrybutu vertex
	glDisableVertexAttribArray(spPBR->a("tangent"));  //Wy��cz przesy�anie danych do atrybutu vertex
	glDisableVertexAttribArray(spPBR->a("bitangent"));  //Wy��cz przesy�anie danych do atrybutu vertex
	glDisableVertexAttribArray(spPBR->a("texture_coordinates"));  //Wy��cz przesy�anie danych do atrybutu texCoord0
}
