#include <cstdlib>
#include <ctime>
#include <vector>
#include <enemy_generator.h>
#include "scene_loader.h"
#include "enemy.h"



void scene_loader::initialize_enemies(const std::string& scene_name) {
    std::srand(std::time(nullptr)); // random seed

    
    int enemy_type_roll = std::rand() % 100 + 1;
    int version_roll = std::rand() % 3 + 1;
    int position_roll = std::rand() % scene_loader::spawn_points.size();

    glm::vec3 random_position = scene_loader::spawn_points[position_roll];
    glm::quat random_rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f); 

    
    if (scene_loader::open_scenes.find(scene_name) != scene_loader::open_scenes.end()) {
        auto& scene = scene_loader::open_scenes[scene_name];

        if (enemy_type_roll <= 10) { // 10% stationary
            if (version_roll == 1) {
                scene.push_back(
                    scripts_system::instantiate<game::enemy, game::enemy::preset, glm::vec3, glm::quat>(
                        game::enemies::stationary1,
                        random_position,
                        random_rotation,
                        "Stationary1_Enemy")
                );
            }
            else if (version_roll == 2) {
                scene.push_back(
                    scripts_system::instantiate<game::enemy, game::enemy::preset, glm::vec3, glm::quat>(
                        game::enemies::stationary2,
                        random_position,
                        random_rotation,
                        "Stationary2_Enemy")
                );
            }
            else {
                scene.push_back(
                    scripts_system::instantiate<game::enemy, game::enemy::preset, glm::vec3, glm::quat>(
                        game::enemies::stationary3,
                        random_position,
                        random_rotation,
                        "Stationary3_Enemy")
                );
            }
        }
        else if (enemy_type_roll <= 30) { // 20% tank
            if (version_roll == 1) {
                scene.push_back(
                    scripts_system::instantiate<game::enemy, game::enemy::preset, glm::vec3, glm::quat>(
                        game::enemies::tank1,
                        random_position,
                        random_rotation,
                        "Tank1_Enemy")
                );
            }
            else if (version_roll == 2) {
                scene.push_back(
                    scripts_system::instantiate<game::enemy, game::enemy::preset, glm::vec3, glm::quat>(
                        game::enemies::tank2,
                        random_position,
                        random_rotation,
                        "Tank2_Enemy")
                );
            }
            else {
                scene.push_back(
                    scripts_system::instantiate<game::enemy, game::enemy::preset, glm::vec3, glm::quat>(
                        game::enemies::tank3,
                        random_position,
                        random_rotation,
                        "Tank3_Enemy")
                );
            }
        }
        else if (enemy_type_roll <= 50) { // 20% kamikaze
            if (version_roll == 1) {
                scene.push_back(
                    scripts_system::instantiate<game::enemy, game::enemy::preset, glm::vec3, glm::quat>(
                        game::enemies::kamikaze1,
                        random_position,
                        random_rotation,
                        "Kamikaze1_Enemy")
                );
            }
            else if (version_roll == 2) {
                scene.push_back(
                    scripts_system::instantiate<game::enemy, game::enemy::preset, glm::vec3, glm::quat>(
                        game::enemies::kamikaze2,
                        random_position,
                        random_rotation,
                        "Kamikaze2_Enemy")
                );
            }
            else {
                scene.push_back(
                    scripts_system::instantiate<game::enemy, game::enemy::preset, glm::vec3, glm::quat>(
                        game::enemies::kamikaze3,
                        random_position,
                        random_rotation,
                        "Kamikaze3_Enemy")
                );
            }
        }
        else if (enemy_type_roll <= 70) { // 20% sniper
            if (version_roll == 1) {
                scene.push_back(
                    scripts_system::instantiate<game::enemy, game::enemy::preset, glm::vec3, glm::quat>(
                        game::enemies::sniper1,
                        random_position,
                        random_rotation,
                        "Sniper1_Enemy")
                );
            }
            else if (version_roll == 2) {
                scene.push_back(
                    scripts_system::instantiate<game::enemy, game::enemy::preset, glm::vec3, glm::quat>(
                        game::enemies::sniper2,
                        random_position,
                        random_rotation,
                        "Sniper2_Enemy")
                );
            }
            else {
                scene.push_back(
                    scripts_system::instantiate<game::enemy, game::enemy::preset, glm::vec3, glm::quat>(
                        game::enemies::sniper3,
                        random_position,
                        random_rotation,
                        "Sniper3_Enemy")
                );
            }
        }
        else { // 30% floater
            if (version_roll == 1) {
                scene.push_back(
                    scripts_system::instantiate<game::enemy, game::enemy::preset, glm::vec3, glm::quat>(
                        game::enemies::floater1,
                        random_position,
                        random_rotation,
                        "Floater1_Enemy")
                );
            }
            else if (version_roll == 2) {
                scene.push_back(
                    scripts_system::instantiate<game::enemy, game::enemy::preset, glm::vec3, glm::quat>(
                        game::enemies::floater2,
                        random_position,
                        random_rotation,
                        "Floater2_Enemy")
                );
            }
            else {
                scene.push_back(
                    scripts_system::instantiate<game::enemy, game::enemy::preset, glm::vec3, glm::quat>(
                        game::enemies::floater3,
                        random_position,
                        random_rotation,
                        "Floater3_Enemy")
                );
            }
        }
    }
    else {
        printf("Scene %s not found.\n", scene_name.c_str());
    }
}
