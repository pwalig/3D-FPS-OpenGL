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

std::vector<scripts_system::script*> scene_loader::scripts;

void scene_loader::setup_example_scene()
{
    scripts.push_back(new game::gameplay_manager());
    scripts.push_back(new game::player());
}

void scene_loader::free()
{
    for (auto it = scene_loader::scripts.begin(); it != scene_loader::scripts.end(); ++it) {
        delete (*it);
    }
    scripts.clear();
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