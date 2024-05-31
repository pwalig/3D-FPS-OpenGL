#pragma once
#include "model.h"
#include "textures.h"

namespace renderer {
    class lambert_textured_model : public renderer::model {
    public:
        lambert_textured_model(const std::string& mesh_, const std::string& texture, const glm::mat4& initialMatrix = glm::mat4(1.0f));
        void draw() override;
        void draw2();
    private:
        texture_ptr texture;
    };

}
