#include "renderer.h"
#include "input_system.h"
#include <glm/gtc/type_ptr.hpp>
#include <engine.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <model.h>
#include <ui_visual.h>

glm::mat4 renderer::V = glm::lookAt(glm::vec3(0.0f, 5.0f, -10.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
glm::mat4 renderer::P = glm::perspective(glm::radians(70.0f), engine::window_width / engine::window_height, 0.2f, 100.0f);


void renderer::draw_scene(GLFWwindow* window) {
	//************Place any code here that draws something inside the window******************l
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Clear color and depth buffers

	//renderer::render_textured(glm::mat4(1.0f), myCubeVertices, myCubeTexCoords, myCubeVertexCount, tex);
    renderer::model::draw_all_models();
	ui_system::ui_visual::draw_ui();
	glfwSwapBuffers(window); //Copy back buffer to the front buffer

	// reseting mouse delta
	input_system::mouse_delta = glm::vec2(0.0, 0.0);

    
}
