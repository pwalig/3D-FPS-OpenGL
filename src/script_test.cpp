#include "script_test.h"
#include <input_system.h>
#include "constants.h"
#include <engine.h>
#include <time_system.h>


void script_test::y_plus() { this->speed_y += PI; }
void script_test::y_minus() { this->speed_y -= PI; }
void script_test::x_plus() { this->speed_x += PI; }
void script_test::x_minus() { this->speed_x -= PI; }

void script_test::start()
{
	input_system::subscribe(std::bind(&script_test::y_plus, this), GLFW_KEY_RIGHT, GLFW_PRESS);
	input_system::subscribe(std::bind(&script_test::y_minus, this), GLFW_KEY_LEFT, GLFW_PRESS);
	input_system::subscribe(std::bind(&script_test::x_plus, this), GLFW_KEY_UP, GLFW_PRESS);
	input_system::subscribe(std::bind(&script_test::x_minus, this), GLFW_KEY_DOWN, GLFW_PRESS);

	input_system::subscribe(std::bind(&script_test::y_plus, this), GLFW_KEY_LEFT, GLFW_RELEASE);
	input_system::subscribe(std::bind(&script_test::y_minus, this), GLFW_KEY_RIGHT, GLFW_RELEASE);
	input_system::subscribe(std::bind(&script_test::x_minus, this), GLFW_KEY_UP, GLFW_RELEASE);
	input_system::subscribe(std::bind(&script_test::x_plus, this), GLFW_KEY_DOWN, GLFW_RELEASE);
}

void script_test::update()
{
	this->angle_x += this->speed_x * time_system::delta_time; //Compute an angle by which the object was rotated during the previous frame
	this->angle_y += this->speed_y * time_system::delta_time; //Compute an angle by which the object was rotated during the previous frame
}
