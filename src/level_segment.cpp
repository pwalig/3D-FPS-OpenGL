#include "level_segment.h"
#include "scene_loader.h"
#include "level_gate.h"

game::level_segment::level_segment(const std::string& scene, const std::vector<std::string>& scene_pool)
{
	scene_loader::load_scene(scene);
	std::vector<game::level_gate*> gates = scene_loader::find_scripts_of_type<game::level_gate>(scene);
}
