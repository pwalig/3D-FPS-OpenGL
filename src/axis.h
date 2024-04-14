#pragma once
#include <glm/fwd.hpp>

namespace input_system {
	class axis {
	private:
		int _plus_key;
		int _minus_key;
		float _state = 0.0f;

		void minus();
		void plus();

		void subscribe();

	public:
		axis(const int& plus_key, const int& minus_key);
		axis(const axis& other);
		axis(const axis&& other) noexcept;
		float state() const;
	};

	class double_axis {
	private:
		axis _x;
		axis _y;

	public:
		double_axis(const int& x_plus_key, const int& x_minus_key, const int& y_plus_key, const int& y_minus_key);
		double_axis(const axis& x, const axis& y);
		glm::vec2 normalized() const;
		glm::vec2 raw() const;
	};

	class triple_axis {
	private:
		axis _x;
		axis _y;
		axis _z;

	public:
		triple_axis(const int& x_plus_key, const int& x_minus_key, const int& y_plus_key, const int& y_minus_key, const int& z_plus_key, const int& z_minus_key);
		triple_axis(const axis& x, const axis& y, const axis& z);
		glm::vec3 normalized() const;
		glm::vec3 raw() const;
	};
}