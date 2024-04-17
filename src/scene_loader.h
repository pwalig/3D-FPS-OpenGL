#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>
#include <string>

namespace scene_loader {
    struct Model {
    public:
        glm::mat4 model_matrix;  // model matrix as a regular variable

        // Constructor (if you want to initialize the matrix during creation)
        Model(const glm::mat4& initialMatrix = glm::mat4(1.0f)) : model_matrix(initialMatrix) {}
    };

    //std::vector<Model> Models;

    std::vector<scene_loader::Model> load_models_from_json(const std::string& filename);
}