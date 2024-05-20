#pragma once
#include <shaderprogram.h>

namespace renderer {
	class material {
	public:
		ShaderProgram* shader;
		virtual void draw() {};
	};
}

