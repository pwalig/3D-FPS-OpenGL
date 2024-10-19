#include <cstdlib>
#include <ctime>
#include <vector>
#include <enemy_generator.h>
#include "enemy.h"
#include "scene_loader.h"
#include "spawn_points.h"
#include <gameplay_manager.h>

float scene_loader::generator::period = 10.0f;

time_system::function_timer* scene_loader::generator::generate_enemy_cooldown = nullptr;

void scene_loader::generator::initialize_enemies(const std::string& scene_name) {
    
    
    int enemy_type_roll = std::rand() % 100 + 1;
    int version_roll = std::rand() % 3 + 1;
    if (game::spawn_point::spawn_points.empty()) return; // when no spawn points available don't spawn
    int position_roll = std::rand() % game::spawn_point::spawn_points.size();

    glm::vec3 random_position = game::spawn_point::spawn_points[position_roll]->coords;
    glm::quat random_rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);

    game::enemy* scr;

    if (enemy_type_roll <= 10) { // 10% stationary
        if (version_roll == 1) {
            scr = scripts_system::instantiate<game::enemy, game::enemy::preset, glm::vec3, glm::quat>(
                game::enemies::stationary1,
                random_position,
                random_rotation,
                "Stationary1_Enemy");
        }
        else if (version_roll == 2) {
            scr = scripts_system::instantiate<game::enemy, game::enemy::preset, glm::vec3, glm::quat>(
                game::enemies::stationary2,
                random_position,
                random_rotation,
                "Stationary2_Enemy");
        }
        else {
            scr = scripts_system::instantiate<game::enemy, game::enemy::preset, glm::vec3, glm::quat>(
                game::enemies::stationary3,
                random_position,
                random_rotation,
                "Stationary3_Enemy");
        }
    }
    else if (enemy_type_roll <= 30) { // 20% tank
        if (version_roll == 1) {
            scr = scripts_system::instantiate<game::enemy, game::enemy::preset, glm::vec3, glm::quat>(
                game::enemies::tank1,
                random_position,
                random_rotation,
                "Tank1_Enemy");
        }
        else if (version_roll == 2) {
            scr = scripts_system::instantiate<game::enemy, game::enemy::preset, glm::vec3, glm::quat>(
                game::enemies::tank2,
                random_position,
                random_rotation,
                "Tank2_Enemy");
        }
        else {
            scr = scripts_system::instantiate<game::enemy, game::enemy::preset, glm::vec3, glm::quat>(
                game::enemies::tank3,
                random_position,
                random_rotation,
                "Tank3_Enemy");
        }
    }
    else if (enemy_type_roll <= 50) { // 20% kamikaze
        if (version_roll == 1) {
            scr = scripts_system::instantiate<game::enemy, game::enemy::preset, glm::vec3, glm::quat>(
                game::enemies::kamikaze1,
                random_position,
                random_rotation,
                "Kamikaze1_Enemy");
        }
        else if (version_roll == 2) {
            scr = scripts_system::instantiate<game::enemy, game::enemy::preset, glm::vec3, glm::quat>(
                game::enemies::kamikaze2,
                random_position,
                random_rotation,
                "Kamikaze2_Enemy");
        }
        else {
            scr = scripts_system::instantiate<game::enemy, game::enemy::preset, glm::vec3, glm::quat>(
                game::enemies::kamikaze3,
                random_position,
                random_rotation,
                "Kamikaze3_Enemy");
        }
    }
    else if (enemy_type_roll <= 70) { // 20% sniper
        if (version_roll == 1) {
            scr = scripts_system::instantiate<game::enemy, game::enemy::preset, glm::vec3, glm::quat>(
                game::enemies::sniper1,
                random_position,
                random_rotation,
                "Sniper1_Enemy");
        }
        else if (version_roll == 2) {
            scr = scripts_system::instantiate<game::enemy, game::enemy::preset, glm::vec3, glm::quat>(
                game::enemies::sniper2,
                random_position,
                random_rotation,
                "Sniper2_Enemy");
        }
        else {
            scr = scripts_system::instantiate<game::enemy, game::enemy::preset, glm::vec3, glm::quat>(
                game::enemies::sniper3,
                random_position,
                random_rotation,
                "Sniper3_Enemy");
        }
    }
    else { // 30% floater
        if (version_roll == 1) {
            scr = scripts_system::instantiate<game::enemy, game::enemy::preset, glm::vec3, glm::quat>(
                game::enemies::floater1,
                random_position,
                random_rotation,
                "Floater1_Enemy");
        }
        else if (version_roll == 2) {
            scr = scripts_system::instantiate<game::enemy, game::enemy::preset, glm::vec3, glm::quat>(
                game::enemies::floater2,
                random_position,
                random_rotation,
                "Floater2_Enemy");
        }
        else {
            scr = scripts_system::instantiate<game::enemy, game::enemy::preset, glm::vec3, glm::quat>(
                game::enemies::floater3,
                random_position,
                random_rotation,
                "Floater3_Enemy");
        }
    }

    scene_loader::move_to_scene(scr, scene_name, false);
}
void scene_loader::generator::schedule_enemy_initialization(const std::string& scene_name) {
    // Wywo³aj initialize_enemies
    initialize_enemies(scene_name);

    // Tworzenie funkcji za pomoc¹ std::bind do ponownego wywo³ania schedule_enemy_initialization
    auto bound_function = std::bind(&scene_loader::generator::schedule_enemy_initialization, scene_name);

    // Inicjalizacja timera z t¹ funkcj¹ i od razu uruchomienie timera na 10 sekund
    generate_enemy_cooldown = new time_system::function_timer(
        game::gameplay_manager::multiply_by_difficulty(period, 0.4f, true),
        bound_function
    );

    generate_enemy_cooldown->die_on_finish = true; // delete timer to free memory after timer executed
}

void scene_loader::generator::init() {
    // Tworzenie funkcji za pomoc¹ std::bind
    schedule_enemy_initialization("enemies");
}


