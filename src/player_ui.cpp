#include "player_ui.h"
#include <glm/ext/matrix_transform.hpp>

game::player_ui::player_ui() : crosshair("../assets/UI/crosshair.png", glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(960.0f, 540.0f, -10.0f)), glm::vec3(7.0f, 7.0f, 7.0f))),
	hp_bar("bricks.png", glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(960.0f, 20.0f, -10.0f)), glm::vec3(400.0f, 25.0f, 1.0f))),
	test_text("test text", "../assets/fonts/bitmap/pixel-mono.png", glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 1080.0f, -10.0f)), glm::vec3(20.0f, 30.0f, 1.0f)))
{
	crosshair.color.a = 0.7f;
	test_text.color = glm::vec4(1.0f, 0.6f, 0.6f, 1.0f);
}
