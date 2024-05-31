#include "damage_number.h"
#include <glm/ext/matrix_transform.hpp>
#include "scripts_system.h"
#include <physics.h>
#include <random_t.h>
#include "time_system.h"

game::damage_number::damage_number(const int& num) : ft(RandomT<float>(0.8f, 1.0f), std::bind(&scripts_system::safe_destroy, this)), rb(), uit(std::to_string(num), "../assets/fonts/bitmap/pixel-mono.png", glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(960.0f, 540.0f, -500.0f)), glm::vec3(30.0f)))
{
	rb.velocity = glm::vec3(RandomT<float>(0.5f, 1.0f), RandomT<float>(0.1f, 0.7f), 0.0f);
	rb.velocity = glm::normalize(rb.velocity) * RandomT<float>(4.8f, 7.2f);
	rb.force = physics::gravity * rb.mass * 0.02f;
}

void game::damage_number::update()
{
	rb.temp_force -= rb.velocity * 10.0f;
	uit.model_matrix = glm::translate(uit.model_matrix, rb.velocity * (float)time_system::delta_time + glm::vec3(0.0f, 0.0f, -1.0f));
	uit.color.a = (ft.time * 2.0f) - 1.0f;
}
