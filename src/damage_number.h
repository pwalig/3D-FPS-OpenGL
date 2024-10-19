#pragma once
#include "pop_text.h"
#include <timer.h>
#include <ui_text.h>
#include <rigidbody.h>

namespace game {
	class damage_number : public game::pop_text {
	public:
		damage_number(const int& num);
	};
}

