#include "damage_number.h"
#include <glm/ext/matrix_transform.hpp>
#include "scripts_system.h"
#include <physics.h>
#include <random_t.h>
#include "time_system.h"

game::damage_number::damage_number(const int& num) : pop_text(std::to_string(num), glm::vec3(0.5f, 0.5f, 0.49f))
{}
