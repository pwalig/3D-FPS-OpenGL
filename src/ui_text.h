#pragma once
#include "ui_visual.h"
#include <string>
namespace ui_system {
    class ui_text : public ui_system::ui_visual {
    public:
        std::string text;
        float width;
        ui_text(const std::string& text_, const char* filename, const glm::mat4& model_matrix_ = glm::mat4(1.0f));
        void draw() override;
    };
}

