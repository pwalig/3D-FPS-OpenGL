#include "player_ui.h"
#include <glm/ext/matrix_transform.hpp>
#include <time_system.h>

game::player_ui::player_ui() : crosshair("../assets/UI/crosshair.png", glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(960.0f, 540.0f, -10.0f)), glm::vec3(7.0f, 7.0f, 7.0f))),
	hp_bar("bricks.png", glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(960.0f, 20.0f, -10.0f)), glm::vec3(400.0f, 25.0f, 1.0f))),
	fps_meter("fps: ", "../assets/fonts/bitmap/pixel-mono.png", glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 1080.0f, -10.0f)), glm::vec3(20.0f, 30.0f, 1.0f)))
{
	crosshair.color.a = 0.7f;
	t.function = [this]() {
		this->fps_meter.text = "fps: " + std::to_string(frames);
		this->frames = 0;
		t.start(1.0f);
		};
	t.start(1.0f);
}

void game::player_ui::update()
{
	++frames;
}
