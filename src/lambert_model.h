#pragma once
#include "model.h"
namespace renderer {
    class lambert_model : public renderer::model {
    public:
        lambert_model(const std::string& mesh_, const glm::mat4& initialMatrix = glm::mat4(1.0f));
        void draw() override;
        void draw2();
    };
}

