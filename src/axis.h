#pragma once
#include <glm/glm.hpp>

namespace input_system {
	float axis_state(const int& plus_key, const int& minus_key);

	class axis {
	private:
		int _plus_key;
		int _minus_key;
		float _state;
		int *_plus_press, *_minus_press, *_plus_release, *_minus_release;

		void minus();
		void plus();

		void subscribe();
		void un_subscribe();

	public:
		axis(const int& plus_key, const int& minus_key);
		axis(const axis& other);
		axis(const axis&& other) noexcept;
		~axis();
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


	class axis2 {
	private:
		int _plus_key;
		int _minus_key;

	public:
		axis2(const int& plus_key, const int& minus_key);
		float state() const;
	};

	class double_axis2 {
	private:
		axis2 _x;
		axis2 _y;

	public:
		double_axis2(const int& x_plus_key, const int& x_minus_key, const int& y_plus_key, const int& y_minus_key);
		double_axis2(const axis2& x, const axis2& y);
		glm::vec2 normalized() const;
		glm::vec2 raw() const;
	};

	class triple_axis2 {
	private:
		axis2 _x;
		axis2 _y;
		axis2 _z;

	public:
		triple_axis2(const int& x_plus_key, const int& x_minus_key, const int& y_plus_key, const int& y_minus_key, const int& z_plus_key, const int& z_minus_key);
		triple_axis2(const axis2& x, const axis2& y, const axis2& z);
		glm::vec3 normalized() const;
		glm::vec3 raw() const;
	};
}