#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include <renderer.h>
#include <scripts_system.h>
#include <map>
#include "model.h"

namespace scene_loader {
    extern std::map<std::string, std::vector<scripts_system::script*>> open_scenes;

    void load_scene(const std::string& file_name);
    void un_load_scene(const std::string& scene_name);
    void free();

    std::vector<renderer::model> load_models_from_json(const std::string& filename);

    std::string get_scene_name(const scripts_system::script* const scr);
    void move_same_scene(scripts_system::script* scr, const scripts_system::script* const host);
    void move_to_scene(scripts_system::script* scr, const std::string& scene_name, const bool& existing = true);
}