#include "textures.h"
#include <lodepng.h>

std::map<std::string, renderer::texture_ptr> renderer::texture_map;

renderer::texture_ptr renderer::get_texture(const std::string& filename)
{
	auto it = renderer::texture_map.find(filename);
	if (it != renderer::texture_map.end()) {
		return it->second;
	}

	GLuint textureID = readTexture(filename.c_str());
	auto texture = std::make_shared<GLuint>(textureID);
	renderer::texture_map[filename] = texture;
	return texture;
}

GLuint renderer::readTexture(const char* filename) {
	GLuint tex;
	glActiveTexture(GL_TEXTURE0);

	//Read into computers memory
	std::vector<unsigned char> image;   //Allocate memory 
	unsigned width, height;   //Variables for image size
	//Read the image
	unsigned error = lodepng::decode(image, width, height, filename);
	
	//Import to graphics card memory
	glGenTextures(1, &tex); //Initialize one handle
	glBindTexture(GL_TEXTURE_2D, tex); //Activate handle
	//Copy image to graphics cards memory reprezented by the active handle
	glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*)image.data());

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return tex;
}

void renderer::free_textures()
{
	for (std::pair<const std::string, renderer::texture_ptr> tmi : texture_map) {
		glDeleteTextures(1, tmi.second.get());
	}
}
