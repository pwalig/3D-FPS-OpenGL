#include "light.h"

std::vector<renderer::light*> renderer::light::all_lights;

renderer::light::light(const glm::vec3& position_, const glm::vec3& color_) : position(position_), color(color_)
{
	renderer::light::all_lights.push_back(this);
}

renderer::light::~light()
{
	std::vector<renderer::light*>::iterator id = std::find(renderer::light::all_lights.begin(), renderer::light::all_lights.end(), this);
	if (id != renderer::light::all_lights.end()) renderer::light::all_lights.erase(id);
}

std::vector<float> renderer::light::get_light_positions()
{
	std::vector<float> out;
	for (renderer::light* l : renderer::light::all_lights) {
		out.push_back(l->position.x);
		out.push_back(l->position.y);
		out.push_back(l->position.z);
	}
	return out;
}

std::vector<float> renderer::light::get_light_colors()
{
	std::vector<float> out;
	for (renderer::light* l : renderer::light::all_lights) {
		out.push_back(l->color.x);
		out.push_back(l->color.y);
		out.push_back(l->color.z);
	}
	return out;
}