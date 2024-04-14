#include "renderer.h"
#include "shaderprogram.h"
#include <glm/gtc/type_ptr.hpp>

glm::mat4x4 renderer::V;
glm::mat4x4 renderer::P = glm::perspective(glm::radians(50.0f), 1.0f, 1.0f, 50.0f);

void renderer::render_textured(glm::mat4 M, const float* const mesh, const float* const uv, const int& n, const GLuint& tex)
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
