#pragma once
#include "model.h"
#include "textures.h"

namespace renderer {
	class pbr_model : public renderer::model {
	public:
		texture_ptr diffuse;
		texture_ptr normal;
		texture_ptr height;

		pbr_model(const std::string& mesh_ = ".. / assets / models / cube.obj", const std::string& normal_map = ".. / assets / textures / Neutral_normal.png", const std::string& diffuse_map = ".. / assets / textures / White_Square.png", const std::string& height_map = ".. / assets / textures / White_Square.png", const glm::mat4& initialMatrix = glm::mat4(1.0f));
		virtual void draw();
	};

}

