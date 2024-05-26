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

		mesh(const std::vector<float>& verts, const std::vector<float>& texs, const std::vector<float>& norms, const std::vector<int>& inds)
			: vertices(verts), texCoords(texs), normals(norms), indices(inds) {
			//calculate_tbn();
		}
		void calculate_tbn();

		static std::map<std::string, renderer::mesh_ptr> mesh_map; //map with all meshes
		static renderer::mesh_ptr get_mesh(const std::string& filename); // gets mesh from map

	private:
		static renderer::mesh_ptr load_mesh_from_file(const std::string& filename);
	};
}

