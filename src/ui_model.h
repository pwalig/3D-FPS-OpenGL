#pragma once
#include "ui_visual.h"
#include "mesh.h"
namespace ui_system {
    class ui_model : public ui_visual {
    public:
        renderer::mesh_ptr mesh;
        renderer::texture_ptr texture;
        ui_model(
            const std::string& mesh = "../assets/models/cube.mesh",
            const std::string& texture = "../assets/textures/White_Square.png",
            const glm::vec3& anchor_point_ = glm::vec3(0.0f),
            const glm::mat4& model_matrix_ = glm::mat4(1.0f),
            const glm::vec3& pivot_point_ = glm::vec3(0.0f)
        );
        void draw() override;
    };
}

