#pragma once
#include"colliders.h"

namespace physics {
	void init();
	bool check_collision(const colliders::aabb& a, const colliders::aabb& b);
	bool check_collision(const colliders::sphere& a, const colliders::sphere& b);
}