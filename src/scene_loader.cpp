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


// available scripts
#include "gameplay_manager.h"
#include "player_script.h"
#include "fly_cam.h"
#include "wall.h"

std::vector<scripts_system::script*> scene_loader::keep_on_un_load;

void scene_loader::load_scene(const std::string& file_name) {
    //un load previous scene
    un_load_scene();

    // load new scene from file
    std::ifstream file(file_name); // open file
    nlohmann::json json;
    file >> json;
    for (auto& entry : json["scripts"])
    {
        // create script instance
        nlohmann::json args = entry["args"];
        if (entry["type"] == "gameplay_manager") { scripts_system::scripts.push_back(new game::gameplay_manager()); }
        if (entry["type"] == "player") { scripts_system::scripts.push_back(new game::player(glm::vec3(args["x"], args["y"], args["z"]), args["rot_y"])); }
        if (entry["type"] == "fly_cam") { scripts_system::scripts.push_back(new game::fly_cam()); }
        if (entry["type"] == "wall") { scripts_system::scripts.push_back(new game::wall(glm::vec3(args["size"]["x"], args["size"]["y"], args["size"]["z"]))); }

        // name the script
        scripts_system::scripts.back()->name = entry["name"];
    }
    file.close(); // close file
    printf("===scene_loaded===\n");
    scripts_system::call_events(SCRIPTS_START);
}

void scene_loader::un_load_scene()
{
    for (auto it = scripts_system::scripts.begin(); it != scripts_system::scripts.end(); ++it) {
        if (std::find(scene_loader::keep_on_un_load.begin(), scene_loader::keep_on_un_load.end(), *it) == scene_loader::keep_on_un_load.end()) { // if script not found in keep_on_un_load then destroy it
            delete (*it);
        }
    }
    scripts_system::scripts = scene_loader::keep_on_un_load; // update vector of pointers to have only pointers that survived
    printf("===scene_un_loaded===\n");
}

void scene_loader::free() {
    scene_loader::keep_on_un_load.clear();
    scene_loader::un_load_scene(); // will destroy all of the scripts since keeep_on_un_load is empty
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