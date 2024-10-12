#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <vector>
#include <string>
#include <renderer.h>
#include <scripts_system.h>
#include <map>
#include "model.h"

namespace scene_loader {
    extern std::map<std::string, std::vector<scripts_system::script*>> open_scenes;

    void load_scene(
        const std::string& file_name,
        const glm::vec3& offset = glm::vec3(0.0f),
        const glm::quat& rotation = glm::quat(glm::vec3(0.0f))
    );
    void load_scene(
        const std::string& file_name,
        const std::string& scene_name,
        const glm::vec3& offset = glm::vec3(0.0f),
        const glm::quat& rotation = glm::quat(glm::vec3(0.0f))
    );
    void un_load_scene(const std::string& scene_name);
    void free();

    std::vector<renderer::model> load_models_from_json(const std::string& filename);

    std::string get_scene_name(const scripts_system::script* const scr);
    void move_same_scene(scripts_system::script* scr, const scripts_system::script* const host);
    void move_to_scene(scripts_system::script* scr, const std::string& scene_name, const bool& existing = true);

    template <typename T>
    T* find_script_of_type(const std::string& scene, const std::string& name); // find of type script by name
    template <typename T>
    std::vector<T*> find_scripts_of_type(const std::string& scene, const std::string& name); // find of type script by name
    template <typename T>
    T* find_script_of_type(const std::string& scene); // find of type script
    template <typename T>
    std::vector<T*> find_scripts_of_type(const std::string& scene); // find of type scripts
}

template <typename T>
inline T* scene_loader::find_script_of_type(const std::string& scene, const std::string& name) {
	return scripts_system::find_script_of_type<T>(scene_loader::open_scenes[scene], name);
}

template <typename T>
inline std::vector <T*> scene_loader::find_scripts_of_type(const std::string& scene, const std::string& name) {
    return scripts_system::find_scripts_of_type<T>(scene_loader::open_scenes[scene], name);
}

template <typename T>
inline T* scene_loader::find_script_of_type(const std::string& scene) {
    return scripts_system::find_script_of_type<T>(scene_loader::open_scenes[scene]);
}

template <typename T>
inline std::vector <T*> scene_loader::find_scripts_of_type(const std::string& scene) {
    return scripts_system::find_scripts_of_type<T>(scene_loader::open_scenes[scene]);
}