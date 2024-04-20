#include "scene_loader.h"
#include <glm/gtc/type_ptr.hpp>
#include "shaderprogram.h"
#include <cube.h>
#include <renderer.h>
#include <engine.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <fstream>
#include <vector>
#include <string>
#include <glm/gtc/matrix_transform.hpp>
#include <nlohmann/json.hpp>
#include <iostream>

#include "gameplay_manager.h"
#include "player_script.h"

std::vector<scripts_system::script*> scene_loader::keep_on_un_load;

// temporary function for testing
void scene_loader::setup_example_scene()
{
    scripts_system::scripts.push_back(new game::gameplay_manager());
    scripts_system::scripts.push_back(new game::player());
}

void scene_loader::load_scene(const std::string& file_name) {
    // to do
}

void scene_loader::un_load_scene()
{
    for (auto it = scripts_system::scripts.begin(); it != scripts_system::scripts.end(); ++it) {
        if (std::find(scene_loader::keep_on_un_load.begin(), scene_loader::keep_on_un_load.end(), *it) == scene_loader::keep_on_un_load.end()) { // if script not found in keep_on_un_load then destroy it
            delete (*it);
        }
    }
    scripts_system::scripts = scene_loader::keep_on_un_load;
}

void scene_loader::free() {
    for (auto it = scene_loader::keep_on_un_load.begin(); it != scene_loader::keep_on_un_load.end(); ++it) {
        delete (*it);
    }
    scene_loader::keep_on_un_load.clear();
    scene_loader::un_load_scene();
}

std::vector<renderer::model> scene_loader::load_models_from_json(const std::string& filename) {
    std::ifstream file(filename);
    nlohmann::json json;
    file >> json;
    std::vector<renderer::model> models;
    for (int i = 0; i < json.size(); i++)
    {
        nlohmann::json j = json[i];
        for (auto& entry : j) {
            renderer::model model;
            model.model_matrix = glm::mat4(1.0f);  // Inicjalizacja macierzy jednostkowej

            for (auto& operation : entry) {
                std::string op = operation["operation"];
                float x = operation["x"];
                float y = operation["y"];
                float z = operation["z"];

                if (op == "rotate") {
                    model.model_matrix = glm::rotate(model.model_matrix, glm::radians(x), glm::vec3(1.0f, 0.0f, 0.0f));
                    model.model_matrix = glm::rotate(model.model_matrix, glm::radians(y), glm::vec3(0.0f, 1.0f, 0.0f));
                    model.model_matrix = glm::rotate(model.model_matrix, glm::radians(z), glm::vec3(0.0f, 0.0f, 1.0f));
                }
                else if (op == "scale") {
                    model.model_matrix = glm::scale(model.model_matrix, glm::vec3(x, y, z));
                }
                else if (op == "translate") {
                    model.model_matrix = glm::translate(model.model_matrix, glm::vec3(x, y, z));
                }
            }

            models.push_back(model);
        }
    }
    //renderer::models = models;
    return models;
}