#include "crosshair_indicator.h"
#include <scripts_system.h>
#include <glm/ext/matrix_transform.hpp>

game::crosshair_indicator::crosshair_indicator(const std::string& img) :
	ft(1.0f, std::bind(&scripts_system::safe_destroy, this)),
	uii(img, glm::vec3(0.5f, 0.5f, 0.505f), glm::scale(glm::mat4(1.0f), glm::vec3(0.029f)))
{}

void game::crosshair_indicator::update()
{
	uii.color.a = (ft.time * 2.0f) - 1.0f;
}
