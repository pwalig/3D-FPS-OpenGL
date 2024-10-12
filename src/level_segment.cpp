#include "level_segment.h"
#include "scene_loader.h"
#include <pbr_model.h>
#include "model_script.h"
#include "scripts_system.h"
#include "collider_scripts.h"

std::vector<std::string> game::level_segment::scene_pool = {
	"../assets/scenes/procedural/seg1.json",
	"../assets/scenes/procedural/seg2.json",
	"../assets/scenes/procedural/seg3.json",
	"../assets/scenes/procedural/seg4.json",
	"../assets/scenes/procedural/seg5.json",
	"../assets/scenes/procedural/seg6.json"
};

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
		new level_segment(*gate, scene);
	}
}

game::level_segment::level_segment(
	game::segment_gate& entry_gate,
	const std::string& entry_scene
) : script(name_from_coords(entry_gate.get_position()))
{
	std::string scene = name_from_coords(entry_gate.get_position());
	if (scene.size() > 26){
		printf("%s: wrong_size: %d\n", this->name.c_str(), (int)scene.size());
		scene = "abc";
	}
	scene_loader::load_scene(
		level_segment::random_from_pool(entry_gate),
		scene,
		entry_gate.get_position(),
		entry_gate.get_rotation() * glm::quat(glm::vec3(1.570796251296997f, 0.0f, 0.0f))
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
		try {
		gate->on_pass2_clear = [entry_gate, entry_scene, gates, gate, scene]() {
			scripts_system::safe_destroy(scripts_system::find_script(entry_scene));
			for (game::segment_gate* g : gates) {
				if (g != gate)
					scripts_system::safe_destroy(
						scripts_system::find_script(name_from_coords(g->get_position()))
					);
			}
				scripts_system::events[SCRIPTS_START].subscribe([entry_gate, scene]() {
					glm::mat4 out = glm::translate(glm::mat4(1.0f), entry_gate.get_position());
					out *= glm::toMat4(entry_gate.get_rotation());
					scene_loader::move_to_scene(
						scripts_system::instantiate<game::model_script, renderer::model*>(
							new renderer::pbr_model(
								"../assets/models/cube.mesh",
								"../assets/textures/Neutral_Normal.png",
								"../assets/textures/backrooms/backrooms_wall_color.png",
								"../assets/textures/White_Square.png",
								"../assets/textures/default_data.png",
								glm::scale(out, entry_gate.get_size())),
							"wall_model"), scene);

					scene_loader::move_to_scene(
						scripts_system::instantiate<game::colliders::box, glm::vec3, glm::quat, glm::vec3>(
							entry_gate.get_position(),
							entry_gate.get_rotation(),
							entry_gate.get_size(), "wall_collider"), scene);
					});
			};
		}
		catch (std::exception e) {
			printf("string too long caught. what: %s\n", e.what());
			/*TO DO - fix the problem*/
		}

		gate->on_pass1_spawn = [gate, gates, scene]() {
			for (game::segment_gate* g : gates) {
				if (g != gate)
					scripts_system::events[SCRIPTS_START].subscribe([g, scene]() {
						new level_segment(*g, scene);
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
	std::string s, out = "";
	s = std::to_string(coords.x);
	s.resize(8);
	out += s + "|";
	s = std::to_string(coords.y);
	s.resize(8);
	out += s + "|";
	s = std::to_string(coords.z);
	s.resize(8);
	out += s;
	return out;
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
	return scene_pool;
}
