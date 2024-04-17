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

    std::vector<scene_loader::Model> loadModelsFromJson(const std::string& filename);
}

namespace renderer {
	extern glm::mat4x4 V; //view matrix
	extern glm::mat4x4 P; //perspective matrix

	void render_textured(const glm::mat4& M, const float* const mesh, const float* const uv, const int& n, const GLuint& tex);
    void draw_scene(std::vector<scene_loader::Model> models);
	void drawCube(const glm::mat4& M);
}
