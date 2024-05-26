#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory> 
#include <map>
#include <vector>
#include <string>


namespace renderer {
	struct mesh {
		std::vector<float> vertices;
		std::vector<float> texCoords;
		std::vector<float> normals;
		std::vector<int> indices;
		std::vector<std::vector<glm::vec4>> tbn;

		mesh(const std::vector<float>& verts, const std::vector<float>& texs, const std::vector<float>& norms, const std::vector<int>& inds)
			: vertices(verts), texCoords(texs), normals(norms), indices(inds) {
			calculate_tbn();
		}
		void calculate_tbn();
	};
	using mesh_ptr = std::shared_ptr<mesh>;
	using texture_ptr = std::shared_ptr<GLuint>;

	struct model {
	public:
		glm::mat4 model_matrix;  // model matrix as a regular variable
		mesh_ptr mesh;
		texture_ptr normal;
		texture_ptr diffuse;
		texture_ptr height;

		model(const glm::mat4& initialMatrix = glm::mat4(1.0f), mesh_ptr meshPtr = nullptr,
			texture_ptr normalPtr = nullptr, texture_ptr diffusePtr = nullptr, texture_ptr heightPtr = nullptr);
		~model();
	};

	extern glm::mat4x4 V; //view matrix
	extern glm::mat4x4 P; //perspective matrix
	extern std::vector<renderer::model*> all_models; //vector of all of the models
	extern std::map<std::string, renderer::mesh_ptr> mesh_map; //map with meshes of all models
	extern std::map<std::string, renderer::texture_ptr> texture_map; // map with texture paths

	
	renderer::mesh_ptr load_mesh_from_file(const std::string& filename);
	renderer::mesh_ptr get_mesh(const std::string& filename);
	void render_model(const renderer::model& mdl);
	void render_model_tex(const renderer::model& mdl);
	void render_textured(const glm::mat4& M, const float* const mesh, const float* const uv, const int& n, const GLuint& tex);
	texture_ptr get_texture(const std::string& filename);
	void draw_each_object(const std::vector<renderer::model*>& models);
	void draw_cube(const glm::mat4& M);
	void draw_scene(GLFWwindow* window);
	
}
