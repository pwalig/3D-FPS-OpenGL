#include "renderer.h"
#include <glm/gtc/type_ptr.hpp>
#include "shaderprogram.h"
#include <cube.h>
#include <engine.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <fstream>
#include <vector>
#include <string>
#include <glm/gtc/matrix_transform.hpp>
#include <nlohmann/json.hpp>
#include <iostream>

glm::mat4 renderer::V = glm::lookAt(glm::vec3(0.0f, 5.0f, -10.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
glm::mat4 renderer::P = glm::perspective(glm::radians(70.0f), engine::window_width / engine::window_height, 1.0f, 50.0f);

void renderer::render_textured(const glm::mat4& M, const float* const mesh, const float* const uv, const int& n, const GLuint& tex)
{
	spTextured->use();

	glUniformMatrix4fv(spTextured->u("P"), 1, false, glm::value_ptr(renderer::P));
	glUniformMatrix4fv(spTextured->u("V"), 1, false, glm::value_ptr(renderer::V));
	glUniformMatrix4fv(spTextured->u("M"), 1, false, glm::value_ptr(M));


	glEnableVertexAttribArray(spTextured->a("vertex"));
	glVertexAttribPointer(spTextured->a("vertex"), 4, GL_FLOAT, false, 0, mesh);

	glEnableVertexAttribArray(spTextured->a("texCoord"));
	glVertexAttribPointer(spTextured->a("texCoord"), 2, GL_FLOAT, false, 0, uv);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex);
	glUniform1i(spTextured->u("tex"), 0);

	glDrawArrays(GL_TRIANGLES, 0, n);

	glDisableVertexAttribArray(spTextured->a("vertex"));
	glDisableVertexAttribArray(spTextured->a("color"));
}

void renderer::drawCube(const glm::mat4& M) {
	spLambert->use();
	glUniformMatrix4fv(spLambert->u("M"), 1, false, glm::value_ptr(M));
	glUniformMatrix4fv(spLambert->u("V"), 1, false, glm::value_ptr(renderer::V));
	glUniformMatrix4fv(spLambert->u("P"), 1, false, glm::value_ptr(renderer::P));
	Models::cube.drawSolid();
}
void renderer::draw_scene(std::vector<scene_loader::Model> models) {
    spLambert->use();
    for (const auto& model : models) {
        glUniformMatrix4fv(spLambert->u("M"), 1, false, glm::value_ptr(model.model_matrix));
        glUniformMatrix4fv(spLambert->u("V"), 1, false, glm::value_ptr(renderer::V));
        glUniformMatrix4fv(spLambert->u("P"), 1, false, glm::value_ptr(renderer::P));
        Models::cube.drawSolid();
    }
}

std::vector<scene_loader::Model> scene_loader::loadModelsFromJson(const std::string& filename) {
    std::ifstream file(filename);
    nlohmann::json json;
    file >> json;
    std::vector<scene_loader::Model> models;
    for (int i=0;i< json.size();i++)
    {
        nlohmann::json j = json[i];
        for (auto& entry : j) {
            scene_loader::Model model;
            model.model_matrix = glm::mat4(1.0f);  // Inicjalizacja macierzy jednostkowej

            for (auto& operation : entry) {
                std::string op = operation["operation"];
                float x = operation["x"];
                float y = operation["y"];
                float z = operation["z"];

                if (op == "rotate") {
                    model.model_matrix = glm::rotate(model.model_matrix, glm::radians(x), glm::vec3(1.0f, 0.0f, 0.0f));
                    model.model_matrix = glm::rotate(model.model_matrix, glm::radians(y), glm::vec3(0.0f, 1.0f, 0.0f));
                    model.model_matrix = glm::rotate(model.model_matrix, glm::radians(z), glm::vec3(0.0f, 0.0f, 1.0f));
                }
                else if (op == "scale") {
                    model.model_matrix = glm::scale(model.model_matrix, glm::vec3(x, y, z));
                }
                else if (op == "translate") {
                    model.model_matrix = glm::translate(model.model_matrix, glm::vec3(x, y, z));
                }
            }

            models.push_back(model);
        }
    }

    return models;
}
