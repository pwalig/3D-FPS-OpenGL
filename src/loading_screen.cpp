#include "loading_screen.h"
#include <glm/ext/matrix_transform.hpp>
#include "shaderprogram.h"
#include <ui_system.h>
#include <glm/gtc/type_ptr.hpp>
#include <engine.h>

game::loading_screen::loading_screen(const std::string& initial_info_text) : 
	loading_text("LOADING", "../assets/fonts/bitmap/handwiriting-readable.png", glm::vec3(0.5f), glm::scale(glm::mat4(1.0f), glm::vec3(0.2f))),
	info_text(initial_info_text, "../assets/fonts/bitmap/handwiriting-readable.png", glm::vec3(0.0f, 0.1f, 0.5f), glm::scale(glm::mat4(1.0f), glm::vec3(0.03f)), glm::vec3(-1.0f, 0.0f, 0.0f))
{
	to_draw.push_back(&loading_text);
	to_draw.push_back(&info_text);
	draw();
}

void game::loading_screen::update_info_text(const std::string& new_progress_text)
{
	info_text.update_text(new_progress_text);
	draw();
}

void game::loading_screen::draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Clear color and depth buffers

	spUI->use();

	glUniformMatrix4fv(spUI->u("P"), 1, false, glm::value_ptr(ui_system::P));

	glEnableVertexAttribArray(spUI->a("vertex"));
	glEnableVertexAttribArray(spUI->a("texCoord"));

	glActiveTexture(GL_TEXTURE0);

	for (ui_system::ui_visual* uiv : to_draw) {
		glUniformMatrix4fv(spUI->u("M"), 1, false, glm::value_ptr(uiv->model_matrix));
		glm::mat4 anchor = glm::translate(glm::mat4(1.0f), glm::vec3(
			uiv->anchor_point.x * engine::width,
			uiv->anchor_point.y * engine::height,
			-uiv->anchor_point.z));
		anchor = glm::scale(anchor, glm::vec3(
			(ui_system::scaling * engine::width) + ((1.0f - ui_system::scaling) * engine::height),
			(ui_system::scaling * engine::width) + ((1.0f - ui_system::scaling) * engine::height),
			1.0f
		));
		glUniformMatrix4fv(spUI->u("A"), 1, false, glm::value_ptr(anchor));
		glUniform3fv(spUI->u("pivot"), 1, glm::value_ptr(uiv->pivot_point));
		glUniform4fv(spUI->u("color"), 1, glm::value_ptr(uiv->color));
		uiv->draw();
	}

	glDisableVertexAttribArray(spUI->a("vertex"));
	glDisableVertexAttribArray(spUI->a("texCoord"));

	glfwSwapBuffers(engine::window); //Copy back buffer to the front buffer  
}
