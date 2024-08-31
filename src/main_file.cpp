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

#include <physics.h>
#include <engine.h>
#include <input_system.h>
#include <renderer.h>
#include <scene_loader.h>
#include <scripts_system.h>
#include <time_system.h>
#include <enemy_generator.h>

#include <nlohmann/json.hpp>
#include <vector>
#include <string>
#include <ui_visual.h>
#include <weapon.h>
#include <gameplay_manager.h>
#include <iostream>


//Error processing callback procedure
void error_callback(int error, const char* description) {
	fputs(description, stderr);
}

//Initialization code procedure
void initOpenGLProgram(GLFWwindow* window) {
	initShaders();
	//************Place any code here that needs to be executed once, at the program start************
	glClearColor(0, 0, 0, 1); //Set color buffer clear color
	glEnable(GL_DEPTH_TEST); //Turn on pixel depth test based on depth buffer
	glEnable(GL_BLEND); // enable alpha blending
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // set up blending funcion
	glfwSetKeyCallback(window, input_system::key_callback);
	glfwSetMouseButtonCallback(window, input_system::mouse_button_callback);
	glfwSetScrollCallback(window, input_system::scroll_callback);
	glfwSetCursorPosCallback(window, input_system::mouse_callback);
	input_system::init_all();
	scripts_system::initialize();
	renderer::mesh::init();
	renderer::texture_resource::init();
	game::weapon::init();
	std::srand(std::time(nullptr)); // random seed
}

//Release resources allocated by the program
void freeOpenGLProgram(GLFWwindow* window) {
	freeShaders();
	//************Place any code here that needs to be executed once, after the main loop ends************

	scene_loader::free();
	game::weapon::free();
	renderer::texture_resource::free_all();
	renderer::mesh::free_all();
	scripts_system::free();
	input_system::free_all();
}


int main(void)
{
	//GLFWwindow* window; //Pointer to object that represents the application window

	glfwSetErrorCallback(error_callback);//Register error processing callback procedure

	if (!glfwInit()) { //Initialize GLFW library
		fprintf(stderr, "Can't initialize GLFW.\n");
		exit(EXIT_FAILURE);
	}

	engine::window = glfwCreateWindow(static_cast<int>(engine::window_width), static_cast<int>(engine::window_height), "OpenGL", NULL, NULL);  //Create a window 500pxx500px titled "OpenGL" and an OpenGL context associated with it. 
	glfwSetFramebufferSizeCallback(engine::window, game::gameplay_manager::framebuffer_size_callback);

	if (!engine::window) //If no window is opened then close the program
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(engine::window); //Since this moment OpenGL context corresponding to the window is active and all OpenGL calls will refer to this context.
	glfwSwapInterval(1); //During vsync wait for the first refresh

	GLenum err;
	if ((err = glewInit()) != GLEW_OK) { //Initialize GLEW library
		fprintf(stderr, "Can't initialize GLEW: %s\n", glewGetErrorString(err));
		exit(EXIT_FAILURE);
	}

	initOpenGLProgram(engine::window); //Call initialization procedure

	//Main application loop
	scene_loader::load_scene("initial_scene.json"); // load scene
	glfwSetTime(0); //clear internal timer
	while (!glfwWindowShouldClose(engine::window)) //As long as the window shouldnt be closed yet...
	{
		time_system::delta_time = glfwGetTime() * time_system::time_scale;
		glfwSetTime(0); //clear internal timer

		scripts_system::update(); // update scripts
#ifdef DEBUG
		double time_sum = glfwGetTime();
		game::debugger::scripts_time += time_sum;
#endif
		
		time_system::timers.perform_on_all([](time_system::timer* t) { t->update(); }); // update timers
#ifdef DEBUG
		game::debugger::timers_time += glfwGetTime() - time_sum;
		time_sum = glfwGetTime();
#endif
		
		physics::run();
#ifdef DEBUG
		game::debugger::physics_time += glfwGetTime() - time_sum;
		time_sum = glfwGetTime();
#endif

		renderer::draw_scene(engine::window); //Execute drawing procedure
#ifdef DEBUG
		game::debugger::render_time += glfwGetTime() - time_sum;
		time_sum = glfwGetTime();
#endif

		input_system::mouse_delta = glm::vec2(0.0, 0.0); // reseting mouse delta
		glfwPollEvents(); //Process callback procedures corresponding to the events that took place up to now
#ifdef DEBUG
		game::debugger::callbacks_time += glfwGetTime() - time_sum;
#endif

		if (game::graphics_menu::framerate_cap_enable) {
			while (glfwGetTime() < 1.0f / game::graphics_menu::framerate_cap) {};
		}
	}
	freeOpenGLProgram(engine::window);

	glfwDestroyWindow(engine::window); //Delete OpenGL context and the window.
	glfwTerminate(); //Free GLFW resources
	exit(EXIT_SUCCESS);
}
