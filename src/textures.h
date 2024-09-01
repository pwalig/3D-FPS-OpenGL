#pragma once
#include <GL/glew.h>
#include <map>
#include <memory>
#include <string>

namespace renderer {
	extern GLuint global_cube_map;

	class texture_resource {
	public:
		texture_resource(const texture_resource& other) = delete;
		texture_resource(texture_resource&& other) = delete;

		GLuint texture;

		void set_delete_on_0_refs(const bool& del);

		static void init();
		static void pre_load(const std::string& filename);
		static void free_all();
		static void print_texture_map_info();

		friend class texture_ptr;

	private:
		texture_resource(const std::string& filename);
		~texture_resource();

		static std::map<std::string, renderer::texture_resource*> texture_map;
		static void erase_resource_from_map(renderer::texture_resource* resource);
		unsigned int refs;
		bool delete_on_0_refs;

		static GLuint load_texture_from_png_file(const std::string& filename); // gets texture directly from file
		static GLuint load_cubemap_from_png_files(const std::string& filename); // filename without last character and .png extension
	};

	class texture_ptr {
	public:
		texture_ptr(const std::string& filename);
		texture_ptr(const texture_ptr& other);
		texture_ptr(texture_ptr&& other) noexcept;
		texture_ptr& operator= (const texture_ptr& other);
		texture_ptr& operator= (texture_ptr&& other) noexcept;
		GLuint operator-> ();
		GLuint get();
		~texture_ptr();

		friend class texture_resource;
	private:
		texture_resource* resource = nullptr;
	};
}