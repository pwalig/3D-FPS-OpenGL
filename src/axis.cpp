#include "axis.h"
#include <input_system.h>
#include <engine.h>
#include <glm/ext/vector_float2.hpp>
#include <glm/geometric.hpp>

// AXIS

void input_system::axis::minus()
{
	this->_state -= 1.0f;
}

void input_system::axis::plus()
{
	this->_state += 1.0f;
}

void input_system::axis::subscribe()
{
	this->_plus_press = input_system::key_events[GLFW_PRESS][this->_plus_key].subscribe(std::bind(&input_system::axis::plus, this));
	this->_minus_press = input_system::key_events[GLFW_PRESS][this->_minus_key].subscribe(std::bind(&input_system::axis::minus, this));
	this->_plus_release = input_system::key_events[GLFW_RELEASE][this->_plus_key].subscribe(std::bind(&input_system::axis::minus, this));
	this->_minus_release = input_system::key_events[GLFW_RELEASE][this->_minus_key].subscribe(std::bind(&input_system::axis::plus, this));
}

void input_system::axis::un_subscribe()
{
	input_system::key_events[GLFW_PRESS][this->_plus_key].unsubscribe(*_plus_press);
	input_system::key_events[GLFW_PRESS][this->_minus_key].unsubscribe(*_minus_press);
	input_system::key_events[GLFW_RELEASE][this->_plus_key].unsubscribe(*_plus_release);
	input_system::key_events[GLFW_RELEASE][this->_minus_key].unsubscribe(*_minus_release);
}

input_system::axis::axis(const int& plus_key, const int& minus_key) : _plus_key(plus_key), _minus_key(minus_key)
{ this->subscribe(); }

input_system::axis::axis(const axis& other) : _plus_key(other._plus_key), _minus_key(other._minus_key)
{ this->subscribe(); }

input_system::axis::axis(const axis&& other) noexcept : _plus_key(other._plus_key), _minus_key(other._minus_key)
{ this->subscribe(); }

input_system::axis::~axis()
{ this->un_subscribe(); }

float input_system::axis::state() const
{
	return this->_state;
}


// DOUBLE AXIS

input_system::double_axis::double_axis(const int& x_plus_key, const int& x_minus_key, const int& y_plus_key, const int& y_minus_key) : _x(x_plus_key, x_minus_key), _y(y_plus_key, y_minus_key) {}

input_system::double_axis::double_axis(const axis& x, const axis& y) : _x(x), _y(y) {}

glm::vec2 input_system::double_axis::normalized() const
{
	glm::vec2 raw = glm::vec2(_x.state(), _y.state());
	if (glm::length(raw) == 0.0f) return raw;
	return glm::normalize(raw);
}

glm::vec2 input_system::double_axis::raw() const
{
	return glm::vec2(_x.state(), _y.state());
}


// TRIPLE AXIS

input_system::triple_axis::triple_axis(const int& x_plus_key, const int& x_minus_key, const int& y_plus_key, const int& y_minus_key, const int& z_plus_key, const int& z_minus_key) : _x(x_plus_key, x_minus_key), _y(y_plus_key, y_minus_key), _z(z_plus_key, z_minus_key) {}

input_system::triple_axis::triple_axis(const axis& x, const axis& y, const axis& z) : _x(x), _y(y), _z(z) {}

glm::vec3 input_system::triple_axis::normalized() const
{
	glm::vec3 raw = glm::vec3(_x.state(), _y.state(), _z.state());
	if (glm::length(raw) == 0.0f) return raw;
	return glm::normalize(raw);
}

glm::vec3 input_system::triple_axis::raw() const
{
	return glm::vec3(_x.state(), _y.state(), _z.state());
}
