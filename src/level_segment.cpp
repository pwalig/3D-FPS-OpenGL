#include "level_segment.h"
#include "scene_loader.h"
#include "level_gate.h"

game::level_segment::level_segment(const std::string& scene, const std::vector<std::string>& scene_pool)
{
	scene_loader::load_scene(scene);
	std::vector<game::level_gate*> gates = scene_loader::find_scripts_of_type<game::level_gate>(scene);
	std::vector<std::string> neighbours = level_segment::random_from_pool(scene_pool, gates.size());
	for (game::level_gate* gate : gates) {
		gate->scenes2 = neighbours;
	}
}

std::string game::level_segment::random_from_pool(const std::vector<std::string>& scene_pool)
{
	return scene_pool[rand() % scene_pool.size()];
}

std::vector<std::string> game::level_segment::random_from_pool(const std::vector<std::string>& scene_pool, const int& amount)
{
	std::vector<std::string> out;
	for (int i = 0; i < amount; ++i) out.push_back(random_from_pool(scene_pool));
	return out;
}
