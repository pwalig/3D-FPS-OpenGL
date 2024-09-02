#include "renderer.h"
#include <glm/gtc/type_ptr.hpp>
#include <engine.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <model.h>
#include <ui_visual.h>

renderer::camera renderer::active_camera;
float renderer::global_fov = 70.0f;
float renderer::gamma = 2.2f;
float renderer::exposure = 1.0f;

void renderer::draw_scene(GLFWwindow* window) {
	//************Place any code here that draws something inside the window******************l
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Clear color and depth buffers

	//renderer::render_textured(glm::mat4(1.0f), myCubeVertices, myCubeTexCoords, myCubeVertexCount, tex);
    renderer::model::draw_all_models();
	ui_system::ui_visual::draw_ui();
	glfwSwapBuffers(window); //Copy back buffer to the front buffer    
}
