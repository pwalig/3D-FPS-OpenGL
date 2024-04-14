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

float speed_x = 0;//[radians/s]
float speed_y = 0;//[radians/s]
GLuint tex;
physics::rigidbody rb1;

void y_plus() { speed_y += PI; }
void y_minus() { speed_y -= PI; }
void x_plus() { speed_x += PI; }
void x_minus() { speed_x -= PI; }

//Error processing callback procedure
void error_callback(int error, const char* description) {
	fputs(description, stderr);
}


void key_callback(
	GLFWwindow* window,
	int key,
	int scancode,
	int action,
	int mod
) {
	if (action == GLFW_PRESS) {
		for (auto _event : input_system::key_pressed_events[key]) {
			_event();
		}
	}
	if (action == GLFW_RELEASE) {
		for (auto _event : input_system::key_released_events[key]) {
			_event();
		}
	}
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
	glfwSetKeyCallback(window, key_callback);
	tex = readTexture("bricks.png");

	input_system::initialize();


	input_system::subscribe(y_plus, GLFW_KEY_RIGHT);
	input_system::subscribe(y_minus, GLFW_KEY_LEFT);
	input_system::subscribe(x_minus, GLFW_KEY_UP);
	input_system::subscribe(x_plus, GLFW_KEY_DOWN);

	input_system::subscribe(y_plus, GLFW_KEY_LEFT, false);
	input_system::subscribe(y_minus, GLFW_KEY_RIGHT, false);
	input_system::subscribe(x_minus, GLFW_KEY_DOWN, false);
	input_system::subscribe(x_plus, GLFW_KEY_UP, false);
}

//Release resources allocated by the program
void freeOpenGLProgram(GLFWwindow* window) {
	freeShaders();
	glDeleteTextures(1, &tex);
	//************Place any code here that needs to be executed once, after the main loop ends************

	input_system::free();
}


void texCube(glm::mat4 P, glm::mat4 V, glm::mat4 M) {

	spTextured->use();

	glUniformMatrix4fv(spTextured->u("P"), 1, false, glm::value_ptr(P));
	glUniformMatrix4fv(spTextured->u("V"), 1, false, glm::value_ptr(V));
	glUniformMatrix4fv(spTextured->u("M"), 1, false, glm::value_ptr(M));


	glEnableVertexAttribArray(spTextured->a("vertex"));
	glVertexAttribPointer(spTextured->a("vertex"), 4, GL_FLOAT, false, 0, myCubeVertices);

	glEnableVertexAttribArray(spTextured->a("texCoord"));
	glVertexAttribPointer(spTextured->a("texCoord"), 2, GL_FLOAT, false, 0, myCubeTexCoords);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex);
	glUniform1i(spTextured->u("tex"), 0);

	glDrawArrays(GL_TRIANGLES, 0, myCubeVertexCount);

	glDisableVertexAttribArray(spTextured->a("vertex"));
	glDisableVertexAttribArray(spTextured->a("color"));
}

//Drawing procedure
void drawScene(GLFWwindow* window, float angle_x, float angle_y) {
	//************Place any code here that draws something inside the window******************l
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Clear color and depth buffers

	rb1.force = physics::gravity * rb1.mass;
	if (rb1.position.y <= -10.0f) { physics::collide(&rb1, 0.7f); rb1.dynamic = false; }
	if (rb1.position.y > -10.0f) rb1.dynamic = true;
	physics::update(&rb1);
	glm::mat4x4 M = glm::rotate(rb1.transform, angle_y, glm::vec3(0.0f, 1.0f, 0.0f)); //Multiply model matrix by the rotation matrix around Y axis by angle_y degrees
	M = glm::rotate(M, angle_x, glm::vec3(1.0f, 0.0f, 0.0f)); //Multiply model matrix by the rotation matrix around X axis by angle_x degrees
	glm::mat4 V = glm::lookAt(glm::vec3(0.0f, 0.0f, -20.0f), rb1.position / 2.0f, glm::vec3(0.0f, 1.0f, 0.0f)); //Compute view matrix
	glm::mat4 P = glm::perspective(glm::radians(50.0f), 1.0f, 1.0f, 50.0f); //Compute projection matrix


	texCube(P, V, M);


	glfwSwapBuffers(window); //Copy back buffer to the front buffer
}

int main(void)
{
	GLFWwindow* window; //Pointer to object that represents the application window

	glfwSetErrorCallback(error_callback);//Register error processing callback procedure

	if (!glfwInit()) { //Initialize GLFW library
		fprintf(stderr, "Can't initialize GLFW.\n");
		exit(EXIT_FAILURE);
	}

	window = glfwCreateWindow(500, 500, "OpenGL", NULL, NULL);  //Create a window 500pxx500px titled "OpenGL" and an OpenGL context associated with it. 

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
	float angle_x = 0; //declare variable for storing current rotation angle
	float angle_y = 0; //declare variable for storing current rotation angle
	glfwSetTime(0); //clear internal timer
	while (!glfwWindowShouldClose(window)) //As long as the window shouldnt be closed yet...
	{
		engine::delta_time = glfwGetTime() * engine::time_scale;
		angle_x += speed_x * glfwGetTime(); //Compute an angle by which the object was rotated during the previous frame
		angle_y += speed_y * glfwGetTime(); //Compute an angle by which the object was rotated during the previous frame
		glfwSetTime(0); //clear internal timer
		drawScene(window, angle_x, angle_y); //Execute drawing procedure
		glfwPollEvents(); //Process callback procedures corresponding to the events that took place up to now
	}
	freeOpenGLProgram(window);

	glfwDestroyWindow(window); //Delete OpenGL context and the window.
	glfwTerminate(); //Free GLFW resources
	exit(EXIT_SUCCESS);
}
