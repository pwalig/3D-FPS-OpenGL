#include "collision_testing.h"
#include <renderer.h>
#include "input_system.h"

physics::collision_test_script::collision_test_script() : rb1(), rb2(), sc1(&rb1, 1.0f), pc(&rb2, glm::vec2(100.0f, 100.0f)) {}

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
	rb1.position = glm::vec3(0.0f, 5.0f, 0.0f);
	rb1.force = physics::gravity * rb1.mass;
	rb1.restitution = 0.6f;
	physics::rigidbodies.push_back(&rb1);
	physics::all_colliders.push_back(&sc1);

	// init rigidbody 2
	rb2.position = glm::vec3(0.0f, -5.0f, 0.0f);
	rb2.restitution = 0.0f;
	physics::rigidbodies.push_back(&rb2);
	physics::all_colliders.push_back(&pc);

	// subscribe to collider
	sc1.on_collision_enter.subscribe([](physics::collision_info other) {printf("enter(%f, %f, %f)\n", other.collision_point.x, other.collision_point.y, other.collision_point.z); });
	sc1.on_collision_stay.subscribe([](physics::collision_info other) {printf("stay(%f, %f, %f)\n", other.collision_point.x, other.collision_point.y, other.collision_point.z); });
	sc1.on_collision_exit.subscribe([]() {printf("exit\n"); });

	// subscribe to input
	input_system::key_events[GLFW_PRESS][GLFW_KEY_F].subscribe(std::bind(&physics::collision_test_script::move_sphere1, this));
	input_system::key_events[GLFW_PRESS][GLFW_KEY_G].subscribe(std::bind(&physics::collision_test_script::move_sphere2, this));
	
	// set up models
	renderer::model m1;
	renderer::model m2;
	m1.model_matrix = rb1.model_matrix();
	m2.model_matrix = glm::scale(rb2.model_matrix(), glm::vec3(50.0f, 0.01f, 50.0f));
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
	m2.model_matrix = glm::scale(rb2.model_matrix(), glm::vec3(50.0f, 0.01f, 50.0f));
	renderer::all_models.push_back(m2);
	renderer::all_models.push_back(m1);
}
