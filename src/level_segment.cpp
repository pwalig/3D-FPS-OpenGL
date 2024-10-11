#include "level_segment.h"
#include "scene_loader.h"

std::vector<std::string> game::level_segment::scene_pool;

game::level_segment::level_segment(
	const std::string& scene_file
) : script(name_from_coords(glm::vec3(0.0f)))
{
	std::string scene = name_from_coords(glm::vec3(0.0f));
	scene_loader::load_scene(
		scene_file,
		scene
	);
	std::vector<game::segment_gate*> gates = scene_loader::find_scripts_of_type<game::segment_gate>(scene);

	for (game::segment_gate* gate : gates) {
		gate->on_pass2_clear = [gates, gate]() {
			for (game::segment_gate* g : gates) {
				if (g != gate) scripts_system::safe_destroy(g);
			}
			};
		new level_segment(*gate, scene_file);
	}
}

game::level_segment::level_segment(
	game::segment_gate& entry_gate,
	const std::string& entry_scene
) : script(name_from_coords(entry_gate.get_position()))
{
	std::string scene = name_from_coords(entry_gate.get_position());
	scene_loader::load_scene(
		level_segment::random_from_pool(entry_gate),
		scene,
		entry_gate.get_position()
	);
	std::vector<game::segment_gate*> gates = scene_loader::find_scripts_of_type<game::segment_gate>(scene);

	entry_gate.on_pass2_spawn = [gates, scene]() {
		for (game::segment_gate* gate : gates) {
			scripts_system::events[SCRIPTS_START].subscribe([gate, scene]() {new level_segment(*gate, scene); });
		}
		};

	entry_gate.on_pass1_clear = [gates]() {
		for (game::segment_gate* g : gates) {
			scripts_system::safe_destroy(
				scripts_system::find_script(name_from_coords(g->get_position()))
			);
		}
		};

	for (game::segment_gate* gate : gates) {
		gate->on_pass2_clear = [entry_scene, gates, gate]() {
			scripts_system::safe_destroy(scripts_system::find_script(entry_scene));
			for (game::segment_gate* g : gates) {
				if (g != gate)
					scripts_system::safe_destroy(
						scripts_system::find_script(name_from_coords(g->get_position()))
					);
			}
			};

		gate->on_pass1_spawn = [gate, gates]() {
			for (game::segment_gate* g : gates) {
				if (g != gate)
					scripts_system::events[SCRIPTS_START].subscribe([g]() {
						new level_segment(*g, name_from_coords(g->get_position()));
						});
			}
			};
	}


	printf("%s segment created\n", this->name.c_str());
}

game::level_segment::~level_segment()
{
	scene_loader::un_load_scene(this->name);
}

std::string game::level_segment::name_from_coords(const glm::vec3& coords)
{
	return std::to_string(coords.x) + std::to_string(coords.y) + std::to_string(coords.z);
}

std::vector<std::string> game::level_segment::random_from_pool(const std::vector<game::segment_gate*>& gates)
{
	std::vector<std::string> out;
	for (game::segment_gate* gate: gates){
		out.push_back(game::level_segment::random_from_pool(*gate));
	}
	return out;
}

std::string game::level_segment::random_from_pool(const game::segment_gate& gate)
{
	std::vector<std::string> scene_pool = game::level_segment::get_pool(gate.get_rotation());
	return scene_pool[rand() % scene_pool.size()];
}

std::vector<std::string> game::level_segment::get_pool(const glm::quat& entry_rotation)
{
	std::vector<std::string> out;
	out.push_back("../assets/scenes/procedural/simple_forward.json");
	return out;
}
