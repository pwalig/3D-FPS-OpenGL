#include "engine.h"

float engine::window_width = 1600.0f;
float engine::window_height = 900.0f;
float engine::width = 1600.0f;
float engine::height = 900.0f;
GLFWwindow* engine::window = nullptr;
bool engine::is_fullscreen = false;
int engine::windowed_xpos = 100;
int engine::windowed_ypos = 100;