#pragma once
#include <vector>
#include <memory>
#include <map>
#include <string>

namespace renderer {
	class mesh {
	public:
		mesh(const std::string& filename);
		mesh(const mesh& other) = delete;
		mesh(mesh&& other) = delete;
		~mesh();

		std::vector<unsigned int> indices;
		std::vector<float> vertices;
		std::vector<float> texCoords;
		std::vector<float> normals;
		std::vector<float> tangents;
		std::vector<float> bitangents;

		void set_delete_on_0_refs(const bool& del);

		static void init();
		static void pre_load(const std::string& filename);
		static void free();

		static void print_mesh_map_info();
		friend class mesh_ptr;
	private:
		static std::map<std::string, renderer::mesh*> mesh_map; //map with all meshes
		static void erase_resource_from_map(const std::string& filename);
		std::string filename;
		unsigned int refs;
		bool delete_on_0_refs;

		void load_mesh_from_obj_file(const std::string& filename);
		void load_mesh_from_mesh_file(const std::string& filename);

		class obj_face {
		public:
			int vIndex[3];
			int nIndex[3];
			int tIndex[3];
			obj_face(std::istringstream& ss);
		};
	};

	class mesh_ptr {
	public:
		mesh_ptr(const std::string& filename);
		mesh_ptr(const mesh_ptr& other);
		mesh_ptr(mesh_ptr&& other) noexcept;
		mesh_ptr& operator= (const mesh_ptr& other);
		mesh_ptr& operator= (mesh_ptr&& other) noexcept;
		mesh* operator-> ();
		mesh& operator* ();
		mesh* get();
		~mesh_ptr();
	private:
		mesh* mesh;
	};
}

