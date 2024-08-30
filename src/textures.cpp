#include "textures.h"
#include <lodepng.h>
#include "debug_defines.h"

namespace renderer {
	std::map<std::string, renderer::texture_resource*> texture_map;
}

GLuint renderer::read_texture(const char* filename) {
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

void renderer::texture_resource::erase_resource_from_map(renderer::texture_resource* resource)
{
	std::map<std::string, renderer::texture_resource*>::iterator it;
	for (it = texture_map.begin(); it != texture_map.end(); it++)
	{
		if (it->second->texture == resource->texture) {
			texture_map.erase(it);
			break;
		}
	}
}

void renderer::free_textures()
{
	for (std::pair<const std::string, renderer::texture_resource*> tex : texture_map) {
		delete (tex.second);
	}
	texture_map.clear();
}

renderer::texture_ptr::texture_ptr(const std::string& filename)
{
	auto it = renderer::texture_map.find(filename);
	if (it != renderer::texture_map.end()) {
		this->resource = it->second;
	}
	else {
		renderer::texture_map[filename] = new texture_resource(filename); // construct texture resource directly in the map
		this->resource = renderer::texture_map[filename];
#ifdef DEBUG
		printf("loaded texture: %s\n", filename.c_str());
#endif // DEBUG
	}
	this->resource->refs++;
}

renderer::texture_ptr::texture_ptr(const texture_ptr& other) : resource(other.resource)
{
	resource->refs++;
}

renderer::texture_ptr::texture_ptr(texture_ptr&& other) noexcept : resource(other.resource)
{
	resource->refs++;
}

renderer::texture_ptr& renderer::texture_ptr::operator=(const texture_ptr& other)
{
	if (this != &other) {
		this->resource = other.resource;
		resource->refs++;
	}
	return *this;
}

renderer::texture_ptr& renderer::texture_ptr::operator=(texture_ptr&& other) noexcept
{
	if (this != &other) {
		this->resource = other.resource;
		resource->refs++;
	}
	return *this;
}

GLuint renderer::texture_ptr::operator->()
{
	return this->resource->texture;
}

GLuint renderer::texture_ptr::get()
{
	return this->resource->texture;
}

renderer::texture_ptr::~texture_ptr()
{
	resource->refs -= 1;
	if (resource->refs <= 0 && resource->delete_on_0_refs) {
		texture_resource::erase_resource_from_map(resource);
		delete resource;
		this->resource = nullptr;
	}
}

renderer::texture_resource::texture_resource(const std::string& filename) :
	texture(read_texture(filename.c_str())), refs(0), // 0 refs because texture map should not count
	delete_on_0_refs(false) {}

renderer::texture_resource::~texture_resource()
{
	glDeleteTextures(1, &texture);
#ifdef DEBUG
	printf("deleted texture: %d\n", (int)texture);
#endif // DEBUG
}

void renderer::texture_resource::set_delete_on_0_refs(const bool& del)
{
	this->delete_on_0_refs = del;

	if (this->refs <= 0 && this->delete_on_0_refs) {
		erase_resource_from_map(this);
		delete this;
	}
}
