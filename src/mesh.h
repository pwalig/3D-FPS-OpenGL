#pragma once
#include <vector>
#include <memory>
#include <map>
#include <string>

namespace renderer {
	class mesh;

	using mesh_ptr = std::shared_ptr<mesh>;

	class mesh {
	public:
		std::vector<float> vertices;
		std::vector<float> texCoords;
		std::vector<float> normals;
		std::vector<int> indices;
		std::vector<float> c1;
		std::vector<float> c2;
		std::vector<float> c3;

		static std::map<std::string, renderer::mesh_ptr> mesh_map; //map with all meshes
		static renderer::mesh_ptr get_mesh(const std::string& filename); // gets mesh from map
		static void init();
	private:
		static renderer::mesh_ptr load_mesh_from_file(const std::string& filename);

		class obj_face {
		public:
			int vIndex[3];
			int nIndex[3];
			int tIndex[3];
			obj_face(std::istringstream& ss);
		};
	};
}

