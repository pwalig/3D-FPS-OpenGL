#include "lambert_model.h"
#include <renderer.h>
#include "shaderprogram.h"
#include <glm/gtc/type_ptr.hpp>
#include <myCube.h>

renderer::lambert_model::lambert_model(const std::string& mesh_, const glm::mat4& initialMatrix) : model(mesh_, initialMatrix) {}


void renderer::lambert_model::draw()
{
	spLambert->use();//Aktywacja programu cieniuj¹cego
    glUniformMatrix4fv(spLambert->u("P"), 1, false, glm::value_ptr(renderer::active_camera.get_P()));
    glUniformMatrix4fv(spLambert->u("V"), 1, false, glm::value_ptr(renderer::active_camera.get_V()));
	glUniformMatrix4fv(spLambert->u("M"), 1, false, glm::value_ptr(this->model_matrix));

	glEnableVertexAttribArray(spLambert->a("vertex"));  //W³¹cz przesy³anie danych do atrybutu vertex
	glVertexAttribPointer(spLambert->a("vertex"), 4, GL_FLOAT, false, 0, this->mesh->vertices.data()); //Wska¿ tablicê z danymi dla atrybutu vertex

	glEnableVertexAttribArray(spLambert->a("normal"));  //W³¹cz przesy³anie danych do atrybutu texCoord
	glVertexAttribPointer(spLambert->a("normal"), 4, GL_FLOAT, false, 0, this->mesh->normals.data()); //Wska¿ tablicê z danymi dla atrybutu texCoord

    if (mesh->indices.empty()) glDrawArrays(GL_TRIANGLES, 0, this->mesh->vertices.size() / 4);
    else glDrawElements(GL_TRIANGLES, this->mesh->indices.size(), GL_UNSIGNED_INT, this->mesh->indices.data());

	glDisableVertexAttribArray(spLambert->a("vertex"));  //Wy³¹cz przesy³anie danych do atrybutu vertex
	glDisableVertexAttribArray(spLambert->a("normal"));  //Wy³¹cz przesy³anie danych do atrybutu texCoord0
}

void renderer::lambert_model::draw2() {
    // Use the shader program (assuming spLambert is your shader program)
    spLambert->use();

    // Set the uniform matrices
    glUniformMatrix4fv(spLambert->u("M"), 1, GL_FALSE, glm::value_ptr(this->model_matrix));
    glUniformMatrix4fv(spLambert->u("P"), 1, false, glm::value_ptr(renderer::active_camera.get_P()));
    glUniformMatrix4fv(spLambert->u("V"), 1, false, glm::value_ptr(renderer::active_camera.get_V()));

    // Generate and bind VAO
    GLuint VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    // Bind and fill vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, this->mesh->vertices.size() * sizeof(float), this->mesh->vertices.data(), GL_STATIC_DRAW);

    // Bind and fill element buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->mesh->indices.size() * sizeof(int), this->mesh->indices.data(), GL_STATIC_DRAW);

    // Set vertex attribute pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Draw the mesh
    glDrawElements(GL_TRIANGLES, this->mesh->indices.size(), GL_UNSIGNED_INT, 0);

    // Unbind VAO
    glBindVertexArray(0);

    // Clean up
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteVertexArrays(1, &VAO);
}
