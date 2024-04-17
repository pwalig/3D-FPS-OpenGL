/*
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#define GLM_FORCE_RADIANS

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdlib.h>
#include <stdio.h>
#include "constants.h"
#include "allmodels.h"
#include "lodepng.h"
#include "shaderprogram.h"
#include "myCube.h"

#include <physics.h>
#include <engine.h>
#include <input_system.h>
#include <script_test.h>
#include <renderer.h>
#include <scene_loader.h>
#include <player_script.h>
#include <fly_cam.h>
#include <scripts_system.h>
#include <time_system.h>

#include <nlohmann/json.hpp>
#include <vector>
#include <string>

GLuint tex;

//Error processing callback procedure
void error_callback(int error, const char* description) {
	fputs(description, stderr);
}

GLuint readTexture(const char* filename) {
	GLuint tex;
	glActiveTexture(GL_TEXTURE0);

	//Read into computers memory
	std::vector<unsigned char> image;   //Allocate memory 
	unsigned width, height;   //Variables for image size
	//Read the image
	unsigned error = lodepng::decode(image, width, height, filename);

	//Import to graphics card memory
	glGenTextures(1, &tex); //Initialize one handle
	glBindTexture(GL_TEXTURE_2D, tex); //Activate handle
	//Copy image to graphics cards memory reprezented by the active handle
	glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*)image.data());

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return tex;
}

//Initialization code procedure
void initOpenGLProgram(GLFWwindow* window) {
	initShaders();
	//************Place any code here that needs to be executed once, at the program start************
	glClearColor(0, 0, 0, 1); //Set color buffer clear color
	glEnable(GL_DEPTH_TEST); //Turn on pixel depth test based on depth buffer
	glfwSetKeyCallback(window, input_system::key_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, input_system::mouse_callback);
	tex = readTexture("bricks.png");
	input_system::init_all();
	scripts_system::initialize();

	// register events - ideally should only register scene loader, scene loader should register the rest
	scripts_system::subscribe(game::player::init, SCRIPTS_INIT);
	//engine::subscribe(game::fly_cam::start, ENGINE_AT_START); //alternative camera
	//engine::subscribe(game::fly_cam::update, ENGINE_AT_UPDATE);

	scripts_system::call_events(SCRIPTS_INIT);
}

//Release resources allocated by the program
void freeOpenGLProgram(GLFWwindow* window) {
	freeShaders();
	glDeleteTextures(1, &tex);
	//************Place any code here that needs to be executed once, after the main loop ends************

	input_system::free_all();
	scripts_system::call_events(SCRIPTS_FREE);
	scripts_system::free();
}


int main(void)
{
	GLFWwindow* window; //Pointer to object that represents the application window

	glfwSetErrorCallback(error_callback);//Register error processing callback procedure

	if (!glfwInit()) { //Initialize GLFW library
		fprintf(stderr, "Can't initialize GLFW.\n");
		exit(EXIT_FAILURE);
	}

	window = glfwCreateWindow(engine::window_width, engine::window_height, "OpenGL", NULL, NULL);  //Create a window 500pxx500px titled "OpenGL" and an OpenGL context associated with it. 

	if (!window) //If no window is opened then close the program
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window); //Since this moment OpenGL context corresponding to the window is active and all OpenGL calls will refer to this context.
	glfwSwapInterval(1); //During vsync wait for the first refresh

	GLenum err;
	if ((err = glewInit()) != GLEW_OK) { //Initialize GLEW library
		fprintf(stderr, "Can't initialize GLEW: %s\n", glewGetErrorString(err));
		exit(EXIT_FAILURE);
	}

	initOpenGLProgram(window); //Call initialization procedure

	//Main application loop
	scripts_system::call_events(SCRIPTS_START);
	glfwSetTime(0); //clear internal timer
	while (!glfwWindowShouldClose(window)) //As long as the window shouldnt be closed yet...
	{
		time_system::delta_time = glfwGetTime() * time_system::time_scale;
		glfwSetTime(0); //clear internal timer

		scripts_system::call_events(SCRIPTS_UPDATE); // update scripts
		time_system::timer_calls.call_events(); // update timers

		renderer::draw_scene(window); //Execute drawing procedure
		glfwPollEvents(); //Process callback procedures corresponding to the events that took place up to now
	}
	freeOpenGLProgram(window);

	glfwDestroyWindow(window); //Delete OpenGL context and the window.
	glfwTerminate(); //Free GLFW resources
	exit(EXIT_SUCCESS);
}
