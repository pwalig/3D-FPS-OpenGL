#include "model.h"
#include <renderer.h>
#include "shaderprogram.h"
#include <cube.h>
#include <glm/gtc/type_ptr.hpp>

std::vector<renderer::model*> renderer::model::all_models;

renderer::model::model(const glm::mat4& initial_matrix) : model_matrix(initial_matrix), mesh(nullptr) {
    all_models.push_back(this);
}

renderer::model::model(const std::string& mesh_, const glm::mat4& initial_matrix) : model_matrix(initial_matrix), mesh(renderer::mesh::get_mesh(mesh_)) {
    all_models.push_back(this);
}

renderer::model::~model() {
    std::vector<renderer::model*>::iterator id = std::find(renderer::model::all_models.begin(), renderer::model::all_models.end(), this);
    if (id != renderer::model::all_models.end()) renderer::model::all_models.erase(id);
}

void draw_cube(const glm::mat4& M) {
    spLambert->use();
    glUniformMatrix4fv(spLambert->u("M"), 1, false, glm::value_ptr(M));
    glUniformMatrix4fv(spLambert->u("V"), 1, false, glm::value_ptr(renderer::active_camera.get_V()));
    glUniformMatrix4fv(spLambert->u("P"), 1, false, glm::value_ptr(renderer::active_camera.get_P()));
    Models::cube.drawSolid();
}

void renderer::model::draw_all_models()
{
    for (renderer::model* model : all_models) {
        if (model->mesh) {
            model->draw();
        }
        else {
            draw_cube(model->model_matrix);
        }
    }
}