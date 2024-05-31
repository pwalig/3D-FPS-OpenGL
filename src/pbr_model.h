#pragma once
#include "model.h"
#include "textures.h"

namespace renderer {
	class pbr_model : public renderer::model {
	public:
		texture_ptr diffuse;
		texture_ptr normal;
		texture_ptr height;

		pbr_model(const std::string& mesh_, const std::string& normal_map, const std::string& diffuse_map, const std::string& height_map, const glm::mat4& initialMatrix = glm::mat4(1.0f));
		virtual void draw();
	};

}

