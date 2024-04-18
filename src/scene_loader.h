#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include <renderer.h>

namespace scene_loader {
    

    

    std::vector<renderer::model> load_models_from_json(const std::string& filename);
}