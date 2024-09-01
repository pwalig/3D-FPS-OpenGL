#include "textures.h"
#include <lodepng.h>
#include "debug_defines.h"
#include <key_bind.h>
#include <GLFW/glfw3.h>

std::map<std::string, renderer::texture_resource*> renderer::texture_resource::texture_map;
GLuint renderer::global_cube_map;

#ifdef DEBUG
input_system::key_bind* texture_map_info_kb;
#endif // DEBUG

GLuint renderer::texture_resource::load_texture_from_png_file(const std::string& filename) {
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

GLuint renderer::texture_resource::load_cubemap_from_png_files(const std::string& filename)
{
	GLuint tex;
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_CUBE_MAP, tex);

	for (int i = 0; i < 6; i++) {
		//Read into computers memory
		std::vector<unsigned char> image;   //Allocate memory 
		unsigned width, height;   //Variables for image size
		//Read the image
		unsigned error = lodepng::decode(image, width, height, filename + std::to_string(i) + ".png");

		//Copy image to graphics cards memory reprezented by the active handle
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
			0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*)image.data()
		);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

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

void renderer::texture_resource::free_all()
{
	for (std::pair<const std::string, renderer::texture_resource*> tex : texture_map) {
		delete (tex.second);
	}
	texture_map.clear();

	glDeleteTextures(1, &global_cube_map);

#ifdef DEBUG
	delete texture_map_info_kb;
#endif // DEBUG
}

void renderer::texture_resource::print_texture_map_info()
{
	printf("texture map info:\nname : refs : delete on 0 refs\n");
	for (std::pair<const std::string, renderer::texture_resource*> m : texture_map) {
		printf("%s : %d : %d\n", m.first.c_str(), (int)(m.second->refs), m.second->delete_on_0_refs ? 1 : 0);
	}
}

renderer::texture_ptr::texture_ptr(const std::string& filename)
{
	auto it = texture_resource::texture_map.find(filename);
	if (it != texture_resource::texture_map.end()) {
		this->resource = it->second;
	}
	else {
		texture_resource::texture_map[filename] = new texture_resource(filename); // construct texture resource directly in the map
		this->resource = texture_resource::texture_map[filename];
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
	texture(load_texture_from_png_file(filename.c_str())), refs(0), // 0 refs because texture map should not count
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

void renderer::texture_resource::init()
{
	global_cube_map = load_cubemap_from_png_files("../assets/cubemaps/backrooms/");
#ifdef DEBUG
	texture_map_info_kb = new input_system::key_bind([]() { renderer::texture_resource::print_texture_map_info(); }, GLFW_KEY_F5, GLFW_PRESS);
#endif // DEBUG
}

void renderer::texture_resource::pre_load(const std::string& filename)
{
	renderer::texture_ptr t(filename);
	t.resource->delete_on_0_refs = false;
}
