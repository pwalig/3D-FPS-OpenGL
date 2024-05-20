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
#include "simple_enemy.h"
#include "fly_cam.h"
#include "wall.h"
#include "player_ui.h"

// test scripts
#include "collision_testing.h"

std::map<std::string, std::vector<scripts_system::script*>> scene_loader::open_scenes;

void scene_loader::load_scene(const std::string& file_name) {
    // double scene load check
    if (scene_loader::open_scenes.find(file_name) != scene_loader::open_scenes.end()) {
        printf("%s already open\n", file_name.c_str());
        return;
    }
    // load new scene from file
    std::ifstream file(file_name); // open file
    nlohmann::json json;
    file >> json;
    for (auto& entry : json["scripts"])
    {
        // create script instance
        nlohmann::json args = entry["args"];
        if (entry["type"] == "load_scene") scene_loader::load_scene(args["scene"]);
        else if (entry["type"] == "gameplay_manager") { open_scenes[file_name].push_back(scripts_system::instantiate<game::gameplay_manager>(entry["name"])); }
        else if (entry["type"] == "player") { open_scenes[file_name].push_back(scripts_system::instantiate<game::player, glm::vec3, float>(glm::vec3(args["x"], args["y"], args["z"]), args["rot_y"], entry["name"])); }
        else if (entry["type"] == "fly_cam") { open_scenes[file_name].push_back(scripts_system::instantiate<game::fly_cam>(entry["name"])); }
        else if (entry["type"] == "wall") { open_scenes[file_name].push_back(scripts_system::instantiate<game::wall, glm::vec3, glm::vec3, glm::vec3>(
            glm::vec3(args["position"]["x"], args["position"]["y"], args["position"]["z"]), 
            glm::vec3(args["rotation"]["x"], args["rotation"]["y"], args["rotation"]["z"]), 
            glm::vec3(args["size"]["x"], args["size"]["y"], args["size"]["z"]), entry["name"])); }
        else if (entry["type"] == "enemy") { open_scenes[file_name].push_back(scripts_system::instantiate<game::simple_enemy, glm::vec3, float>(glm::vec3(args["x"], args["y"], args["z"]), args["rot_y"], entry["name"])); }
        else if (entry["type"] == "player_ui") { open_scenes[file_name].push_back(scripts_system::instantiate<game::player_ui>(entry["name"])); }
        else if (entry["type"] == "collision_test_script") { open_scenes[file_name].push_back(scripts_system::instantiate<physics::collision_test_script>(entry["name"])); }
    }
    file.close(); // close file
    printf("=== %s loaded===\n", file_name.c_str());
}

void _un_load_scene(const std::string& scene_name) {
    for (auto it = scene_loader::open_scenes[scene_name].begin(); it != scene_loader::open_scenes[scene_name].end(); ++it) {
        if (scripts_system::scripts.on_list(*it)) { // if script is still present in script_system
            scripts_system::destroy(*it); // destroy it
        }
    }
}

void scene_loader::un_load_scene(const std::string& scene_name)
{
    if (scene_loader::open_scenes.find(scene_name) == scene_loader::open_scenes.end()) {
        printf("%s not found in open scenes\n", scene_name.c_str());
    }
    else {
        _un_load_scene(scene_name);
        scene_loader::open_scenes.erase(scene_name);
        printf("=== %s un_loaded ===\n", scene_name.c_str());
    }
}

void scene_loader::free() {
    for (auto const& it : scene_loader::open_scenes) {
        _un_load_scene(it.first);
        printf("=== %s un_loaded ===\n", it.first.c_str());
    } // un_load all scenes
    scene_loader::open_scenes.clear();
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

std::string scene_loader::get_scene_name(const scripts_system::script* const scr) // tells what scene is the script in
{
    for (const auto scene : scene_loader::open_scenes) {
        if (std::find(scene.second.begin(), scene.second.end(), scr) != scene.second.end()) {
            return scene.first;
        }
    }
    return "no_scene";
}
