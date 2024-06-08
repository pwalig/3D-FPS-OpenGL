#include "crosshair_indicator.h"
#include <scripts_system.h>
#include <glm/ext/matrix_transform.hpp>

game::crosshair_indicator::crosshair_indicator(const std::string& img) : ft(1.0f, std::bind(&scripts_system::safe_destroy, this)), uii(img.c_str(), glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(960.0f, 540.0f, -500.0f)), glm::vec3(30.0f)))
{}

void game::crosshair_indicator::update()
{
	uii.color.a = (ft.time * 2.0f) - 1.0f;
}
