#include "camera.h"
#include <glm/gtx/quaternion.hpp>

void renderer::camera::recalculate_P()
{
	this->P = glm::perspective(glm::radians(this->fov), this->aspect_ratio, this->near, this->far);
}

void renderer::camera::recalculate_V()
{
	this->V = glm::lookAt(this->position, this->look_at, this->up_vector);
}

renderer::camera::camera(const glm::vec3& new_position, const glm::vec3& new_look_at, const glm::vec3& new_up_vector, const float& new_fov, const float& new_aspect_ratio, const float& new_near, const float& new_far)
{
	this->set_V(new_position, new_look_at, new_up_vector);
	this->set_P(new_fov, new_aspect_ratio, new_near, new_far);
}

renderer::camera::camera()
{
	this->recalculate_V();
	this->recalculate_P();
}

void renderer::camera::set_V(const glm::vec3& new_position, const glm::vec3& new_look_at, const glm::vec3& new_up_vector)
{
	this->position = new_position;
	this->look_at = new_look_at;
	this->up_vector = new_up_vector;
	this->recalculate_V();
}

void renderer::camera::set_V(const glm::vec3& new_position, const glm::quat& new_rotation)
{
	this->position = new_position;
	this->look_at = this->position + (new_rotation * glm::vec3(0.0f, 0.0f, 1.0f));
	this->up_vector = new_rotation * glm::vec3(0.0f, 1.0f, 0.0f);
	this->recalculate_V();
}

void renderer::camera::set_position(const glm::vec3& new_position)
{
	this->position = new_position;
	this->recalculate_V();
}

void renderer::camera::set_look_at(const glm::vec3& new_look_at, const glm::vec3& new_up_vector)
{
	this->look_at = new_look_at;
	this->up_vector = new_up_vector;
	this->recalculate_V();
}

void renderer::camera::set_direction(const glm::vec3& new_direction, const glm::vec3& new_up_vector)
{
	this->look_at = this->position + new_direction;
	this->up_vector = new_up_vector;
	this->recalculate_V();
}

void renderer::camera::set_rotation(const glm::quat& new_rotation)
{
	this->look_at = this->position + (new_rotation * glm::vec3(0.0f, 0.0f, 1.0f));
	this->up_vector = new_rotation * glm::vec3(0.0f, 1.0f, 0.0f);
	this->recalculate_V();
}

void renderer::camera::set_P(const float& new_fov, const float& new_aspect_ratio, const float& new_near, const float& new_far)
{
	this->fov = new_fov;
	this->aspect_ratio = new_aspect_ratio;
	this->near = new_near;
	this->far = new_far;
	this->recalculate_P();
}

void renderer::camera::set_fov(const float& new_fov)
{
	this->fov = new_fov;
	this->recalculate_P();
}

void renderer::camera::set_aspect_ratio(const float& new_aspect_ratio)
{
	this->aspect_ratio = new_aspect_ratio;
	this->recalculate_P();
}

void renderer::camera::set_clipping(const float& new_near, const float& new_far)
{
	this->near = new_near;
	this->far = new_far;
	this->recalculate_P();
}

glm::mat4 renderer::camera::get_P()
{
	return this->P;
}

glm::mat4 renderer::camera::get_V()
{
	return this->V;
}
