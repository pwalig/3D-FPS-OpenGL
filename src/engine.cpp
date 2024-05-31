#include "engine.h"

float engine::window_width = 1000.0f;
float engine::window_height = 600.0f;
GLFWwindow* engine::window = nullptr;
bool engine::pause = false;
bool engine::is_fullscreen = false;
int engine::windowed_xpos = 100;
int engine::windowed_ypos = 100;