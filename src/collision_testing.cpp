#include "collision_testing.h"
#include <renderer.h>
#include "input_system.h"

physics::collision_test_script::collision_test_script() : rb1(), rb2(), sc1(&rb1, 1.0f), sc2(&rb2, 1.0f) {}

void physics::collision_test_script::move_sphere1() {
	rb2.rotation = glm::rotate(glm::quat(glm::vec3(0.0f)), glm::vec3(0.0f, 0.0f, 0.01f));
}

void physics::collision_test_script::move_sphere2()
{
	rb2.rotation = glm::rotate(glm::quat(glm::vec3(0.0f)), glm::vec3(0.0f, 0.0f, -0.01f));
}

void physics::collision_test_script::start()
{
	// init rigidbody 1
	rb1.position = glm::vec3(2.0f, 2.7f, 0.0f);
	rb1.velocity = glm::vec3(-0.0f, 0.0f, 0.0f);
	rb1.force = physics::gravity * rb1.mass;
	//rb1.force = physics::gravity * rb1.mass;
	rb1.restitution = 0.6f;

	// init rigidbody 2
	rb2.position = glm::vec3(-2.0f, 3.0f, 0.0f);
	rb2.velocity = glm::vec3(5.0f, 0.0f, 0.0f);
	rb2.force = physics::gravity * rb2.mass;
	rb2.restitution = 0.6f;

	// subscribe to collider
	/*
	sc1.on_collision_enter.subscribe([](physics::collision_info other) {printf("enter(%f, %f, %f)\n", other.collision_point.x, other.collision_point.y, other.collision_point.z); });
	sc1.on_collision_stay.subscribe([](physics::collision_info other) {printf("stay(%f, %f, %f)\n", other.collision_point.x, other.collision_point.y, other.collision_point.z); });
	sc1.on_collision_exit.subscribe([]() {printf("exit\n"); });
	*/

	// subscribe to input
	input_system::key_events[GLFW_PRESS][GLFW_KEY_F].subscribe(std::bind(&physics::collision_test_script::move_sphere1, this));
	input_system::key_events[GLFW_PRESS][GLFW_KEY_G].subscribe(std::bind(&physics::collision_test_script::move_sphere2, this));
	
	// set up models
	renderer::model m1;
	renderer::model m2;
	m1.model_matrix = rb1.model_matrix();
	m2.model_matrix = rb2.model_matrix();
	renderer::all_models.push_back(m2);
	renderer::all_models.push_back(m1);
}

void physics::collision_test_script::update()
{
	// update models
	renderer::model m1 = renderer::all_models.back();
	renderer::all_models.pop_back();
	renderer::model m2 = renderer::all_models.back();
	renderer::all_models.pop_back();
	m1.model_matrix = rb1.model_matrix();
	m2.model_matrix = rb2.model_matrix();
	renderer::all_models.push_back(m2);
	renderer::all_models.push_back(m1);
}
