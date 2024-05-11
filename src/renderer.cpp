#include "renderer.h"
#include "input_system.h"
#include <glm/gtc/type_ptr.hpp>
#include "shaderprogram.h"
#include <cube.h>
#include <engine.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <fstream>
#include <vector>
#include <string>
#include <scene_loader.h>
#include <glm/gtc/matrix_transform.hpp>
#include <nlohmann/json.hpp>
#include <iostream>

glm::mat4 renderer::V = glm::lookAt(glm::vec3(0.0f, 5.0f, -10.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
glm::mat4 renderer::P = glm::perspective(glm::radians(70.0f), engine::window_width / engine::window_height, 0.2f, 100.0f);
std::vector<renderer::model*> renderer::all_models;


// MODEL
renderer::model::model(const glm::mat4& initialMatrix) : model_matrix(initialMatrix) {
	renderer::all_models.push_back(this);
}

renderer::model::~model() {
	std::vector<renderer::model*>::iterator id = std::find(renderer::all_models.begin(), renderer::all_models.end(), this);
	if (id != renderer::all_models.end()) renderer::all_models.erase(id);
}


// RENDERER

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

void renderer::draw_cube(const glm::mat4& M) {
	spLambert->use();
	glUniformMatrix4fv(spLambert->u("M"), 1, false, glm::value_ptr(M));
	glUniformMatrix4fv(spLambert->u("V"), 1, false, glm::value_ptr(renderer::V));
	glUniformMatrix4fv(spLambert->u("P"), 1, false, glm::value_ptr(renderer::P));
	Models::cube.drawSolid();
}

void renderer::draw_each_object(std::vector<renderer::model*> models) {
    for (const auto& model : models) {
        renderer::draw_cube(model->model_matrix);
    }
}

void renderer::draw_scene(GLFWwindow* window) {
	//************Place any code here that draws something inside the window******************l
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Clear color and depth buffers

	//renderer::render_textured(glm::mat4(1.0f), myCubeVertices, myCubeTexCoords, myCubeVertexCount, tex);
	renderer::draw_each_object(renderer::all_models);
	glfwSwapBuffers(window); //Copy back buffer to the front buffer

	// reseting mouse delta
	input_system::mouse_delta = glm::vec2(0.0, 0.0);
}




