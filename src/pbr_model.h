#pragma once
#include "model.h"
#include "textures.h"

namespace renderer {
	class pbr_model : public renderer::model {
	public:
		texture_ptr albedo;
		texture_ptr normal;
		texture_ptr height;
		texture_ptr data;

		glm::vec3 albedo_ = glm::vec3(1.0f);
		float roughness_ = 1.0f;
		float metallic_ = 1.0f;
		float emission_ = 1.0f;
		float ao_ = 1.0f;
		glm::vec3 ambient_ = glm::vec3(0.0f);

		pbr_model(
			const std::string& mesh_ = "../assets/models/cube.mesh",
			const std::string& normal_map = "../assets/textures/Neutral_normal.png",
			const std::string& albedo_map = "../assets/textures/White_Square.png",
			const std::string& height_map = "../assets/textures/White_Square.png",
			const std::string& data_map = "../assets/textures/default_data.png",
			const glm::mat4& initialMatrix = glm::mat4(1.0f)
		);
		virtual void draw();
	};

}

