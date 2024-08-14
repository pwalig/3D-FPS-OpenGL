#include "lambert_textured_model.h"
#include <renderer.h>
#include "shaderprogram.h"
#include <glm/gtc/type_ptr.hpp>


renderer::lambert_textured_model::lambert_textured_model(const std::string& mesh_, const std::string& texture_, const glm::mat4& initialMatrix)
    : model(mesh_, initialMatrix), texture(texture_) {}

void renderer::lambert_textured_model::draw()
{
    spLambertTextured->use();//Aktywacja programu cieniuj�cego
    glUniformMatrix4fv(spLambertTextured->u("P"), 1, false, glm::value_ptr(renderer::active_camera.get_P()));
    glUniformMatrix4fv(spLambertTextured->u("V"), 1, false, glm::value_ptr(renderer::active_camera.get_V()));
    glUniformMatrix4fv(spLambertTextured->u("M"), 1, false, glm::value_ptr(this->model_matrix));

    glEnableVertexAttribArray(spLambertTextured->a("vertex"));  //W��cz przesy�anie danych do atrybutu vertex
    glVertexAttribPointer(spLambertTextured->a("vertex"), 4, GL_FLOAT, false, 0, this->mesh->vertices.data()); //Wska� tablic� z danymi dla atrybutu vertex

    glEnableVertexAttribArray(spLambertTextured->a("texCoord"));
    glVertexAttribPointer(spLambertTextured->a("texCoord"), 2, GL_FLOAT, false, 0, this->mesh->texCoords.data());

    glEnableVertexAttribArray(spLambertTextured->a("normal"));  //W��cz przesy�anie danych do atrybutu texCoord
    glVertexAttribPointer(spLambertTextured->a("normal"), 4, GL_FLOAT, false, 0, this->mesh->normals.data()); //Wska� tablic� z danymi dla atrybutu texCoord

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, this->texture.get());
    glUniform1i(spLambertTextured->u("tex"), 0);
    if (mesh->indices.empty()) glDrawArrays(GL_TRIANGLES, 0, this->mesh->vertices.size() / 4);
    else glDrawElements(GL_TRIANGLES, this->mesh->indices.size(), GL_UNSIGNED_INT, this->mesh->indices.data());

    glDisableVertexAttribArray(spLambertTextured->a("vertex"));  //Wy��cz przesy�anie danych do atrybutu vertex
    glDisableVertexAttribArray(spLambertTextured->a("normal"));  //Wy��cz przesy�anie danych do atrybutu texCoord0
    glDisableVertexAttribArray(spLambertTextured->a("color"));
}

void renderer::lambert_textured_model::draw2() {
    // Use the shader program (assuming spLambert is your shader program)
    spLambert->use();

    // Set the uniform matrices
    glUniformMatrix4fv(spLambertTextured->u("M"), 1, GL_FALSE, glm::value_ptr(this->model_matrix));
    glUniformMatrix4fv(spLambertTextured->u("V"), 1, GL_FALSE, glm::value_ptr(renderer::active_camera.get_V()));
    glUniformMatrix4fv(spLambertTextured->u("P"), 1, GL_FALSE, glm::value_ptr(renderer::active_camera.get_P()));

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
