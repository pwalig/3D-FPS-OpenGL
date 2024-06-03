#pragma once
#include <glm/glm.hpp>
#include <vector>

namespace renderer {
    class light {
    public:
        glm::vec3 position;
        glm::vec3 color;

        light(const glm::vec3& position_ = glm::vec3(0.0f, 0.0f, 0.0f), const glm::vec3& color_ = glm::vec3(1.0f, 1.0f, 1.0f));
        ~light();

        static std::vector<renderer::light*> all_lights;
        static std::vector<float> get_light_positions();
        static std::vector<float> get_light_colors();
    };
}

