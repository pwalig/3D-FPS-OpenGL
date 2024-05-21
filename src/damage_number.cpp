#include "damage_number.h"
#include <glm/ext/matrix_transform.hpp>
#include "scripts_system.h"
#include <cmath>
#include <physics.h>

template  <typename T>
T RandomT(const T& _min, const T& _max) {
	return _min + static_cast<T>(rand()) / (static_cast<T>(RAND_MAX / (_max - _min)));
}

game::damage_number::damage_number(const int& num) : t(), rb(), uit(std::to_string(num), "../assets/fonts/bitmap/pixel-mono.png", glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(960.0f, 540.0f, -500.0f)), glm::vec3(30.0f)))
{
	rb.velocity = glm::vec3(RandomT<float>(0.5f, 1.0f), RandomT<float>(0.1f, 0.7f), 0.0f);
	rb.velocity = glm::normalize(rb.velocity) * RandomT<float>(0.08f, 0.12f);
	rb.force = physics::gravity * rb.mass * 0.02f;
	t.events.subscribe(std::bind(&scripts_system::safe_destroy, this));
	t.start(RandomT<float>(0.8f, 1.0f));
}

void game::damage_number::update()
{
	rb.temp_force = -rb.velocity * 10.0f;
	uit.model_matrix = glm::translate(uit.model_matrix, rb.velocity + glm::vec3(0.0f, 0.0f, -1.0f));
	uit.color.a = (t.time * 2.0f) - 1.0f;
}
