#include "level_segment.h"
#include "scene_loader.h"

std::vector<std::string> game::level_segment::scene_pool;

game::level_segment::level_segment(
	const std::string& scene_file,
	const std::string& scene_name,
	game::gate& entry
) : script("level_segment:" + name_from_coords(entry.get_position()))
{
	scene_loader::load_scene(scene_file, scene_name, entry.get_position());
	std::vector<game::gate*> gates = scene_loader::find_scripts_of_type<game::gate>(scene_name);
	std::vector<std::string> neighbours = level_segment::random_from_pool(scene_pool, gates.size());
	int i = 0;
	for (game::gate* gate : gates) {
		gate->on_pass = [gate, neighbours, i](game::gate::state s) {
			new level_segment(neighbours[i], name_from_coords(gate->get_position()), *gate);
			};
		++i;
	}
}

std::string game::level_segment::name_from_coords(const glm::vec3& coords)
{
	return std::to_string(coords.x) + std::to_string(coords.y) + std::to_string(coords.z);
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
