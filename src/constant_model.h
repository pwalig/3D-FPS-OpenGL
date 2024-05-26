#pragma once
#include "model.h"
namespace renderer {
	class constant_model : public model {
	public:
		constant_model(const std::string& mesh_, const glm::mat4& initialMatrix = glm::mat4(1.0f));
		void draw() override;
	};
}

