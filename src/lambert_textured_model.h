#pragma once
#include "model.h"
#include <GL/glew.h>
namespace renderer {
    class lambert_textured_model : public renderer::model {
    public:
        lambert_textured_model(const std::string& mesh_, const GLuint* texture_, const glm::mat4& initialMatrix = glm::mat4(1.0f));
        void draw() override;
        void draw2();
    private:
        const GLuint* texture;
    };

}
