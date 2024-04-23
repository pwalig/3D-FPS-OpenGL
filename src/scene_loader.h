#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include <renderer.h>
#include <scripts_system.h>

namespace scene_loader {
    extern std::vector<scripts_system::script*> keep_on_un_load;

    void load_scene(const std::string& file_name);
    void un_load_scene();
    void free();

    std::vector<renderer::model> load_models_from_json(const std::string& filename);
}