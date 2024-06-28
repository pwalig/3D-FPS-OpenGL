#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

#define VEC3_UP			glm::vec3(0.0f, 1.0f, 0.0f)
#define VEC3_DOWN		glm::vec3(0.0f, -1.0f, 0.0f)
#define VEC3_LEFT		glm::vec3(1.0f, 0.0f, 0.0f)
#define VEC3_RIGHT		glm::vec3(-1.0f, 0.0f, 0.0f)
#define VEC3_FORWARD	glm::vec3(0.0f, 0.0f, 1.0f)
#define VEC3_BACKWARD	glm::vec3(0.0f, 0.0f, -1.0f)
#define QUAT_ZERO		glm::quat(glm::vec3(0.0f))

inline glm::quat rotatation_between(const glm::vec3& v1, const glm::vec3& v2) {
	float angle = glm::acos(glm::dot(v1, v2));
	//printf("%f\n", angle);
	if (angle != 0.0f) return glm::normalize(glm::rotate(QUAT_ZERO, angle, glm::normalize(glm::cross(v1, v2))));
	return QUAT_ZERO;
}