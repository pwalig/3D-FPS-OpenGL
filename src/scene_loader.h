#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include <renderer.h>
#include <scripts_system.h>
#include <map>

namespace scene_loader {
    extern std::map<std::string, std::vector<scripts_system::script*>> open_scenes;

    void load_scene(const std::string& file_name);
    void un_load_scene(const std::string& scene_name);
    void free();

    std::vector<renderer::model> load_models_from_json(const std::string& filename);
}