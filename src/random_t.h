#pragma once
#include <cmath>

template  <typename T>
inline T RandomT(const T& _min, const T& _max) {
	if (_min == _max) return _min;
	return _min + static_cast<T>(rand()) / (static_cast<T>(RAND_MAX / (_max - _min)));
}
