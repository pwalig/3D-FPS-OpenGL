#include "collision_testing.h"
#include "input_system.h"

physics::collision_test_script::collision_test_script() : pos1(glm::vec3(0.0f, 0.0f, 0.0f)), pos2(glm::vec3(0.0f, 0.0f, 0.0f)), sc1(pos1, 1.0f), sc2(pos2, 1.0f){}

void physics::collision_test_script::move_sphere1() {
	this->pos2 = glm::vec3(0.0f, 1.95f, 0.0f);
	printf("%f, %f, %f\n", this->pos2.x, this->pos2.y, this->pos2.z);
}

void physics::collision_test_script::move_sphere2()
{
	this->pos2 = glm::vec3(0.0f, 2.5f, 0.0f);
	printf("%f, %f, %f\n", this->pos2.x, this->pos2.y, this->pos2.z);
}

void physics::collision_test_script::start()
{
	this->pos2 = glm::vec3(0.0f, 2.5f, 0.0f);
	sc1.on_collision_enter.subscribe([](physics::collision_info other) {printf("enter(%f, %f, %f)\n", other.collision_point.x, other.collision_point.y, other.collision_point.z); });
	sc1.on_collision_stay.subscribe([](physics::collision_info other) {printf("stay(%f, %f, %f)\n", other.collision_point.x, other.collision_point.y, other.collision_point.z); });
	sc1.on_collision_exit.subscribe([]() {printf("exit\n"); });
	physics::all_colliders.push_back(&sc1);
	physics::all_colliders.push_back(&sc2);

	input_system::key_events[GLFW_PRESS][GLFW_KEY_F].subscribe(std::bind(&physics::collision_test_script::move_sphere1, this));
	input_system::key_events[GLFW_PRESS][GLFW_KEY_G].subscribe(std::bind(&physics::collision_test_script::move_sphere2, this));
}
