#include <spawn_points.h>
#include <enemy_generator.h>

std::vector<game::spawn_point*> game::spawn_point::spawn_points;

game::spawn_point::spawn_point(const glm::vec3& position) : coords(position) {
	game::spawn_point::spawn_points.push_back(this);
}

game::spawn_point::~spawn_point() {
	std::vector<game::spawn_point*>::iterator id = std::find(game::spawn_point::spawn_points.begin(), game::spawn_point::spawn_points.end(), this);
	if (id != game::spawn_point::spawn_points.end()) game::spawn_point::spawn_points.erase(id);
}