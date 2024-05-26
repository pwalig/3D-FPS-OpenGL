#pragma once
#include <GL/glew.h>
#include <map>
#include <memory>
#include <string>

namespace renderer {
	using texture_ptr = std::shared_ptr<GLuint>;

	extern std::map<std::string, renderer::texture_ptr> texture_map; // map with texture paths
	texture_ptr get_texture(const std::string& filename); // gets texture from map
	GLuint readTexture(const char* filename); // gets texture directly from file
}