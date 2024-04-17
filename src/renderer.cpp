#include "renderer.h"
#include <glm/gtc/type_ptr.hpp>
#include "shaderprogram.h"
#include <cube.h>
#include <engine.h>

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
