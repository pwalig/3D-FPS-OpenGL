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
#include <glm/gtx/quaternion.hpp>
#include <nlohmann/json.hpp>
#include <iostream>
#include <loading_screen.h>

// available scripts
#include "gameplay_manager.h"
#include "player_script.h"
#include "enemy.h"
#include "fly_cam.h"
#include "wall.h"
#include "player_ui.h"
#include "level_gate.h"
#include <collider_scripts.h>
#include <spawn_points.h>
#include <pbr_model.h>
#include "title_screen.h"

// test scripts
#include "collision_testing.h"
#include <dummy.h>
#include <model_script.h>
#include <level_segment.h>
#include <segment_gate.h>
#include <checkpoint.h>

std::map<std::string, std::vector<scripts_system::script*>> scene_loader::open_scenes;

// LOAD FROM FILE HELPER FUNCTIONS

glm::vec3 vec3_from_args(const nlohmann::json& args) {
    return glm::vec3(args["x"], args["y"], args["z"]);
}

glm::quat quat_from_args(const nlohmann::json& args) {
    return glm::quat(glm::vec3(args["x"], args["y"], args["z"]));
}

glm::mat4 mat4_from_args(
    const nlohmann::json& position,
    const nlohmann::json& rotation,
    const nlohmann::json& scale,
    const glm::vec3& offset = glm::vec3(0.0f)
) {
    glm::mat4 out = glm::translate(glm::mat4(1.0f), vec3_from_args(position) + offset);
    out *= glm::toMat4(quat_from_args(rotation));
    return glm::scale(out, vec3_from_args(scale));
}

void scene_loader::load_scene(const std::string& file_name, const glm::vec3& offset, const glm::quat& rotation)
{
    scene_loader::load_scene(file_name, file_name, offset, rotation);
}

void scene_loader::load_scene(
    const std::string& file_name,
    const std::string& scene_name,
    const glm::vec3& offset,
    const glm::quat& rotation
) {
    // double scene load check
    if (scene_loader::open_scenes.find(scene_name) != scene_loader::open_scenes.end()) {
#ifdef _DEBUG
        printf("%s already open\n", scene_name.c_str());
#endif
        return;
    }
    // update text on loading screen
    scripts_system::script* ls1 = scripts_system::find_script("loading_screen");
    if (ls1)
        if (game::loading_screen* ls = dynamic_cast<game::loading_screen*>(ls1))
            ls->update_info_text("loading " + file_name);

    // load new scene from file
    std::ifstream file(file_name); // open file
    nlohmann::json json;
    file >> json;
    for (auto& entry : json["scripts"])
    {
        nlohmann::json args = entry["args"];

        // load scene directive
        if (entry["type"] == "load_scene") {
            scene_loader::load_scene(args["scene"]);
            continue;
        }

        // create script instance
        else if (entry["type"] == "spawn_point") {
            open_scenes[scene_name].push_back(
                scripts_system::instantiate<game::spawn_point, glm::vec3>(
                    rotation * vec3_from_args(args["position"]) + offset,
                    entry["name"])
            );
        }
        else if (entry["type"] == "model") {
            glm::mat4 out = glm::translate(glm::mat4(1.0f), rotation * vec3_from_args(args["position"]) + offset);
            out *= glm::toMat4(rotation * quat_from_args(args["rotation"]));

            open_scenes[scene_name].push_back(
                scripts_system::instantiate<game::model_script, renderer::model*>(
                    new renderer::pbr_model(
                        args["mesh"],
                        args["normal"],
                        args["diffuse"],
                        args["height"],
                        args["data"],
                        glm::scale(out, vec3_from_args(args["size"]))),
                    entry["name"])
            );
        }
        else if (entry["type"] == "gameplay_manager") { open_scenes[scene_name].push_back(scripts_system::instantiate<game::gameplay_manager>(entry["name"])); }
        else if (entry["type"] == "player") { open_scenes[scene_name].push_back(scripts_system::instantiate<game::player, glm::vec3, float>(glm::vec3(args["x"], args["y"], args["z"]), args["rot_y"], entry["name"])); }
        else if (entry["type"] == "fly_cam") { open_scenes[scene_name].push_back(scripts_system::instantiate<game::fly_cam>(entry["name"])); }
        else if (entry["type"] == "wall") { open_scenes[scene_name].push_back(scripts_system::instantiate<game::wall, glm::vec3, glm::vec3, glm::vec3>(
            glm::vec3(args["position"]["x"], args["position"]["y"], args["position"]["z"]), 
            glm::vec3(args["rotation"]["x"], args["rotation"]["y"], args["rotation"]["z"]), 
            glm::vec3(args["size"]["x"], args["size"]["y"], args["size"]["z"]), entry["name"])); }
        else if (entry["type"] == "enemies::floater1") {
            open_scenes[scene_name].push_back(
                scripts_system::instantiate<game::enemy, game::enemy::preset, glm::vec3, glm::quat>(
                    game::enemies::floater1,
                    rotation * vec3_from_args(args["position"]) + offset,
                    rotation * quat_from_args(args["rotation"]),
                    entry["name"])
            );
        }
        else if (entry["type"] == "enemies::floater2") {
            open_scenes[scene_name].push_back(
                scripts_system::instantiate<game::enemy, game::enemy::preset, glm::vec3, glm::quat>(
                    game::enemies::floater2,
                    rotation * vec3_from_args(args["position"]) + offset,
                    rotation * quat_from_args(args["rotation"]),
                    entry["name"])
            );
        }
        else if (entry["type"] == "enemies::floater3") {
            open_scenes[scene_name].push_back(
                scripts_system::instantiate<game::enemy, game::enemy::preset, glm::vec3, glm::quat>(
                    game::enemies::floater3,
                    rotation * vec3_from_args(args["position"]) + offset,
                    rotation * quat_from_args(args["rotation"]),
                    entry["name"])
            );
        }
        else if (entry["type"] == "enemies::stationary1") {
            open_scenes[scene_name].push_back(
                scripts_system::instantiate<game::enemy, game::enemy::preset, glm::vec3, glm::quat>(
                    game::enemies::stationary1,
                    rotation * vec3_from_args(args["position"]) + offset,
                    rotation * quat_from_args(args["rotation"]),
                    entry["name"])
            );
        }
        else if (entry["type"] == "enemies::stationary2") {
            open_scenes[scene_name].push_back(
                scripts_system::instantiate<game::enemy, game::enemy::preset, glm::vec3, glm::quat>(
                    game::enemies::stationary2,
                    rotation * vec3_from_args(args["position"]) + offset,
                    rotation * quat_from_args(args["rotation"]),
                    entry["name"])
            );
        }
        else if (entry["type"] == "enemies::stationary3") {
            open_scenes[scene_name].push_back(
                scripts_system::instantiate<game::enemy, game::enemy::preset, glm::vec3, glm::quat>(
                    game::enemies::stationary3,
                    rotation * vec3_from_args(args["position"]) + offset,
                    rotation * quat_from_args(args["rotation"]),
                    entry["name"])
            );
        }
        else if (entry["type"] == "enemies::kamikaze1") {
            open_scenes[scene_name].push_back(
                scripts_system::instantiate<game::enemy, game::enemy::preset, glm::vec3, glm::quat>(
                    game::enemies::kamikaze1,
                    rotation * vec3_from_args(args["position"]) + offset,
                    rotation * quat_from_args(args["rotation"]),
                    entry["name"])
            );
        }
        else if (entry["type"] == "enemies::kamikaze2") {
            open_scenes[scene_name].push_back(
                scripts_system::instantiate<game::enemy, game::enemy::preset, glm::vec3, glm::quat>(
                    game::enemies::kamikaze2,
                    rotation * vec3_from_args(args["position"]) + offset,
                    rotation* quat_from_args(args["rotation"]),
                    entry["name"])
            );
        }
        else if (entry["type"] == "enemies::kamikaze3") {
            open_scenes[scene_name].push_back(
                scripts_system::instantiate<game::enemy, game::enemy::preset, glm::vec3, glm::quat>(
                    game::enemies::kamikaze3,
                    rotation* vec3_from_args(args["position"]) + offset,
                    rotation* quat_from_args(args["rotation"]),
                    entry["name"])
            );
        }
        else if (entry["type"] == "enemies::sniper1") {
            open_scenes[scene_name].push_back(
                scripts_system::instantiate<game::enemy, game::enemy::preset, glm::vec3, glm::quat>(
                    game::enemies::sniper1,
                    rotation* vec3_from_args(args["position"]) + offset,
                    rotation* quat_from_args(args["rotation"]),
                    entry["name"])
            );
        }
        else if (entry["type"] == "enemies::sniper2") {
            open_scenes[scene_name].push_back(
                scripts_system::instantiate<game::enemy, game::enemy::preset, glm::vec3, glm::quat>(
                    game::enemies::sniper2,
                    rotation * vec3_from_args(args["position"]) + offset,
                    rotation * quat_from_args(args["rotation"]),
                    entry["name"])
            );
        }
        else if (entry["type"] == "enemies::sniper3") {
            open_scenes[scene_name].push_back(
                scripts_system::instantiate<game::enemy, game::enemy::preset, glm::vec3, glm::quat>(
                    game::enemies::sniper3,
                    rotation* vec3_from_args(args["position"]) + offset,
                    rotation* quat_from_args(args["rotation"]),
                    entry["name"])
            );
        }
        else if (entry["type"] == "enemies::tank1") {
            open_scenes[scene_name].push_back(
                scripts_system::instantiate<game::enemy, game::enemy::preset, glm::vec3, glm::quat>(
                    game::enemies::tank1,
                    rotation* vec3_from_args(args["position"]) + offset,
                    rotation* quat_from_args(args["rotation"]),
                    entry["name"])
            );
        }
        else if (entry["type"] == "enemies::tank2") {
            open_scenes[scene_name].push_back(
                scripts_system::instantiate<game::enemy, game::enemy::preset, glm::vec3, glm::quat>(
                    game::enemies::tank2,
                    rotation* vec3_from_args(args["position"]) + offset,
                    rotation* quat_from_args(args["rotation"]),
                    entry["name"])
            );
        }
        else if (entry["type"] == "enemies::tank3") {
            open_scenes[scene_name].push_back(
                scripts_system::instantiate<game::enemy, game::enemy::preset, glm::vec3, glm::quat>(
                    game::enemies::tank3,
                    rotation* vec3_from_args(args["position"]) + offset,
                    rotation* quat_from_args(args["rotation"]),
                    entry["name"])
            );
        }
        else if (entry["type"] == "level_gate") {
            open_scenes[scene_name].push_back(
                scripts_system::instantiate<game::level_gate, glm::vec3, glm::quat, glm::vec3, std::vector<std::string>, std::vector<std::string>>(
                    rotation* vec3_from_args(args["position"]) + offset,
                    rotation* quat_from_args(args["rotation"]),
                    vec3_from_args(args["size"]),
                    args["scenes1"], args["scenes2"], entry["name"]
                )
            );
        }
        else if (entry["type"] == "segment_gate") {
            open_scenes[scene_name].push_back(
                scripts_system::instantiate<game::segment_gate, glm::vec3, glm::quat, glm::vec3>(
                    rotation* vec3_from_args(args["position"]) + offset,
                    rotation* quat_from_args(args["rotation"]),
                    vec3_from_args(args["size"]),
                    entry["name"]
                )
            );
        }
        else if (entry["type"] == "level_segment") {
            open_scenes[scene_name].push_back(
                new game::level_segment(args["scene_file"])
            );
        }
        else if (entry["type"] == "checkpoint") {
            open_scenes[scene_name].push_back(
                scripts_system::instantiate<game::checkpoint, glm::vec3, float>(
                    rotation * vec3_from_args(args["position"]) + offset,
                    vec3_from_args(args["size"]).x,
                    entry["name"]
                )
            );
        }
        else if (entry["type"] == "collectable_cube") {
            open_scenes[scene_name].push_back(
                scripts_system::instantiate<game::collectable_cube, glm::vec3, float>(
                    rotation * vec3_from_args(args["position"]) + offset,
                    vec3_from_args(args["size"]).x,
                    entry["name"]
                )
            );
        }
        else if (entry["type"] == "sphere_collider") {
            open_scenes[scene_name].push_back(
                scripts_system::instantiate<game::colliders::sphere, glm::vec3, float>(
                    rotation * vec3_from_args(args["position"]) + offset,
                    vec3_from_args(args["size"]).x,
                    entry["name"]
                )
            );
        }
        else if (entry["type"] == "aabb_collider") {
            open_scenes[scene_name].push_back(
                scripts_system::instantiate<game::colliders::aabb, glm::vec3, glm::vec3>(
                    rotation * vec3_from_args(args["position"]) + offset,
                    vec3_from_args(args["size"]),
                    entry["name"]
                )
            );
        }
        else if (entry["type"] == "box_collider") {
            open_scenes[scene_name].push_back(
                scripts_system::instantiate<game::colliders::box, glm::vec3, glm::quat, glm::vec3>(
                    rotation * vec3_from_args(args["position"]) + offset,
                    rotation* quat_from_args(args["rotation"]),
                    vec3_from_args(args["size"]),
                    entry["name"]
                )
            );
        }
        else if (entry["type"] == "capsule_collider") {
            open_scenes[scene_name].push_back(
                scripts_system::instantiate<game::colliders::capsule, glm::vec3, glm::quat, float, float>(
                    rotation* vec3_from_args(args["position"]) + offset,
                    rotation* quat_from_args(args["rotation"]),
                    vec3_from_args(args["size"]).x,
                    vec3_from_args(args["size"]).z,
                    entry["name"]
                )
            );
        }
        else if (entry["type"] == "plane_collider") {
            open_scenes[scene_name].push_back(
                scripts_system::instantiate<game::colliders::plane, glm::vec3, glm::quat, glm::vec3>(
                    rotation* vec3_from_args(args["position"]) + offset,
                    rotation* quat_from_args(args["rotation"]),
                    vec3_from_args(args["size"]),
                    entry["name"]
                )
            );
        }
        else if (entry["type"] == "dummy") { open_scenes[scene_name].push_back(scripts_system::instantiate<game::dummy>(entry["name"])); }
        else if (entry["type"] == "player_ui") { open_scenes[scene_name].push_back(scripts_system::instantiate<game::player_ui>(entry["name"])); }
        else if (entry["type"] == "title_screen") { open_scenes[scene_name].push_back(new game::title_screen()); }
        else if (entry["type"] == "collision_test_script") { open_scenes[scene_name].push_back(scripts_system::instantiate<physics::collision_test_script>(entry["name"])); }

        //open_scenes[scene_name].back()->name = entry["name"]; // name script instance
    }
    file.close(); // close file
#ifdef _DEBUG
    printf("=== %s loaded as %s ===\n", file_name.c_str(), scene_name.c_str());
#endif
}

void _un_load_scene(const std::string& scene_name) {
    for (auto it = scene_loader::open_scenes[scene_name].begin(); it != scene_loader::open_scenes[scene_name].end(); ++it) {
        if (scripts_system::scripts.on_list(*it)) { // if script is still present in script_system
            scripts_system::destroy(*it); // destroy it
        }
    }
#ifdef _DEBUG
    printf("=== %s un_loaded ===\n", scene_name.c_str());
#endif
}

void scene_loader::un_load_scene(const std::string& scene_name)
{
    if (scene_loader::open_scenes.find(scene_name) == scene_loader::open_scenes.end()) {
#ifdef _DEBUG
        printf("%s not found in open scenes\n", scene_name.c_str());
#endif
    }
    else {
        _un_load_scene(scene_name);
        scene_loader::open_scenes.erase(scene_name);
    }
}

void scene_loader::free() {
    for (auto const& it : scene_loader::open_scenes) {
        _un_load_scene(it.first);
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

void scene_loader::move_same_scene(scripts_system::script* scr, const scripts_system::script* const host)
{
    std::string scene_to = get_scene_name(host);
    move_to_scene(scr, scene_to);
}

void scene_loader::move_to_scene(scripts_system::script* scr, const std::string& scene_name, const bool& existing)
{
    std::string scene_from = get_scene_name(scr);
    if (scene_name == scene_from) return; // if on same scene -> done!

    // remove scr from scene it currently is in
    auto id = std::find(open_scenes[scene_from].begin(), open_scenes[scene_from].end(), scr);
    if (id != open_scenes[scene_from].end()) open_scenes[scene_from].erase(id);

    if (open_scenes.find(scene_name) != open_scenes.end() || !existing) {
        open_scenes[scene_name].push_back(scr);
    }
}
