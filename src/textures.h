#pragma once
#include <GL/glew.h>
#include <map>
#include <memory>
#include <string>

namespace renderer {
	class texture_resource {
	public:
		texture_resource(const std::string& filename);
		texture_resource(const texture_resource& other) = delete;
		texture_resource(texture_resource&& other) = delete;
		GLuint texture;

		~texture_resource();
		void set_delete_on_0_refs(const bool& del);

		friend class texture_ptr;

	private:
		static void erase_resource_from_map(renderer::texture_resource* resource);
		unsigned int refs;
		bool delete_on_0_refs;
	};

	class texture_ptr {
	public:
		texture_ptr(const std::string& filename);
		texture_ptr(const texture_ptr& other);
		texture_ptr(texture_ptr&& other) noexcept;
		texture_ptr& operator= (const texture_ptr& other);
		texture_ptr& operator= (texture_ptr&& other) noexcept;
		GLuint get();
		~texture_ptr();
	private:
		texture_resource* resource = nullptr;
	};

	// extern std::map<std::string, renderer::texture_ptr> texture_map; // map with texture paths
	GLuint read_texture(const char* filename); // gets texture directly from file

	void free_textures();
}