#pragma once
#include <game_script.h>

class script_test : public engine::script {
public:
	virtual void start();
	virtual void update();

	float angle_x = 0.0f; //declare variable for storing current rotation angle
	float angle_y = 0.0f; //declare variable for storing current rotation angle

private:
	float speed_x = 0.0f;
	float speed_y = 0.0f;

	void y_plus();
	void y_minus();
	void x_plus();
	void x_minus();
};