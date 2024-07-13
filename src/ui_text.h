#pragma once
#include "ui_visual.h"
#include <string>
namespace ui_system {
    class ui_text : public ui_system::ui_visual {
    public:
        std::string text;
        float width;
        renderer::texture_ptr font;
        ui_text(
            const std::string& text_,
            const std::string& font_,
            const glm::vec3& anchor_point_ = glm::vec3(0.0f),
            const glm::mat4& model_matrix_ = glm::mat4(1.0f),
            const glm::vec3& pivot_point_ = glm::vec3(0.0f)
        );
        void draw() override;
    };
}

