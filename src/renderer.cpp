#include "renderer.h"
#include "input_system.h"
#include <glm/gtc/type_ptr.hpp>
#include "shaderprogram.h"
#include <cube.h>
#include <engine.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <fstream>
#include <memory> 
#include <map>
#include <vector>
#include <string>
#include <scene_loader.h>
#include <glm/gtc/matrix_transform.hpp>
#include <nlohmann/json.hpp>
#include <iostream>
#include <ui_visual.h>
#include <sstream>
#include <read_texture.h>

glm::mat4 renderer::V = glm::lookAt(glm::vec3(0.0f, 5.0f, -10.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
glm::mat4 renderer::P = glm::perspective(glm::radians(70.0f), engine::window_width / engine::window_height, 0.2f, 100.0f);
std::vector<renderer::model*> renderer::all_models;
std::map<std::string, renderer::mesh_ptr> renderer::mesh_map;
std::map<std::string, renderer::texture_ptr> renderer::texture_map;


// MODEL
renderer::model::model(const glm::mat4& initialMatrix, mesh_ptr meshPtr, texture_ptr normalPtr, texture_ptr diffusePtr, texture_ptr heightPtr)
    : model_matrix(initialMatrix), mesh(meshPtr), normal(normalPtr), diffuse(diffusePtr), height(heightPtr) {
    renderer::all_models.push_back(this);
}

renderer::model::~model() {
	std::vector<renderer::model*>::iterator id = std::find(renderer::all_models.begin(), renderer::all_models.end(), this);
	if (id != renderer::all_models.end()) renderer::all_models.erase(id);
}

// MESH MAP

renderer::mesh_ptr renderer::load_mesh_from_file(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Cannot open file: " << filename << std::endl;
        return nullptr;
    }

    std::vector<float> vertices;
    std::vector<float> texCoords_temp;
    std::vector<float> normals_temp;
    std::vector<float> texCoords;
    std::vector<float> normals;
    std::vector<int> indices;
    std::string line;

    while (std::getline(file, line)) {
        std::istringstream ss(line);
        std::string type;
        ss >> type;

        if (type == "v") {
            float x, y, z;
            ss >> x >> y >> z;
            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);
        }
        else if (type == "vt") {
            float u, v;
            ss >> u >> v;
            texCoords_temp.push_back(u);
            texCoords_temp.push_back(v);
        }
        else if (type == "vn") {
            float nx, ny, nz;
            ss >> nx >> ny >> nz;
            normals_temp.push_back(nx);
            normals_temp.push_back(ny);
            normals_temp.push_back(nz);
        }
        else if (type == "f") {
            std::string vertex1, vertex2, vertex3;
            int vIndex[3], tIndex[3], nIndex[3];
            char slash;

            ss >> vertex1 >> vertex2 >> vertex3;

            std::replace(vertex1.begin(), vertex1.end(), '/', ' ');
            std::replace(vertex2.begin(), vertex2.end(), '/', ' ');
            std::replace(vertex3.begin(), vertex3.end(), '/', ' ');

            std::istringstream v1(vertex1);
            std::istringstream v2(vertex2);
            std::istringstream v3(vertex3);

            v1 >> vIndex[0] >> tIndex[0] >> nIndex[0];
            v2 >> vIndex[1] >> tIndex[1] >> nIndex[1];
            v3 >> vIndex[2] >> tIndex[2] >> nIndex[2];

            for (int i = 0; i < 3; ++i) {
                indices.push_back(vIndex[i] - 1);
                tIndex[i] -= 1;
                texCoords.push_back(tIndex[i]*2);
                texCoords.push_back(tIndex[i]*2 + 1);
                nIndex[i] -= 1;
                normals.push_back(tIndex[i]*3);
                normals.push_back(tIndex[i]*3 + 1);
                normals.push_back(tIndex[i]*3 + 2);
            }
        }
    }

    return std::make_shared<renderer::mesh>(vertices, texCoords, normals, indices);
}

renderer::mesh_ptr renderer::get_mesh(const std::string& filename) {
    auto it = renderer::mesh_map.find(filename);
    if (it != renderer::mesh_map.end()) {
        return it->second;
    }

    renderer::mesh_ptr mesh = load_mesh_from_file(filename);
    if (mesh) {
        renderer::mesh_map[filename] = mesh;
    }
    return mesh;
}

void renderer::render_model(const renderer::model& mdl) {
    // Use the shader program (assuming spLambert is your shader program)
    spLambert->use();

    // Set the uniform matrices
    glUniformMatrix4fv(spLambert->u("M"), 1, GL_FALSE, glm::value_ptr(mdl.model_matrix));
    glUniformMatrix4fv(spLambert->u("V"), 1, GL_FALSE, glm::value_ptr(renderer::V));
    glUniformMatrix4fv(spLambert->u("P"), 1, GL_FALSE, glm::value_ptr(renderer::P));

    // Generate and bind VAO
    GLuint VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    // Bind and fill vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, mdl.mesh->vertices.size() * sizeof(float), mdl.mesh->vertices.data(), GL_STATIC_DRAW);

    // Bind and fill element buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mdl.mesh->indices.size() * sizeof(int), mdl.mesh->indices.data(), GL_STATIC_DRAW);

    // Set vertex attribute pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Draw the mesh
    glDrawElements(GL_TRIANGLES, mdl.mesh->indices.size(), GL_UNSIGNED_INT, 0);

    // Unbind VAO
    glBindVertexArray(0);

    // Clean up
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteVertexArrays(1, &VAO);
}

// TEXTURE MAP

renderer::texture_ptr renderer::get_texture(const std::string& filename) {
    auto it = renderer::texture_map.find(filename);
    if (it != renderer::texture_map.end()) {
        return it->second;
    }

    GLuint textureID = readTexture(filename.c_str());
    auto texture = std::make_shared<GLuint>(textureID);
    renderer::texture_map[filename] = texture;
    return texture;
}

// RENDERER

void renderer::render_model_tex(const renderer::model& mdl) {
    spTextured->use();

    glUniformMatrix4fv(spTextured->u("P"), 1, GL_FALSE, glm::value_ptr(renderer::P));
    glUniformMatrix4fv(spTextured->u("V"), 1, GL_FALSE, glm::value_ptr(renderer::V));
    glUniformMatrix4fv(spTextured->u("M"), 1, GL_FALSE, glm::value_ptr(mdl.model_matrix));

    GLuint VAO, VBO, EBO, texCoordVBO = 0, normalVBO = 0;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, mdl.mesh->vertices.size() * sizeof(float), mdl.mesh->vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mdl.mesh->indices.size() * sizeof(int), mdl.mesh->indices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(spTextured->a("vertex"));
    glVertexAttribPointer(spTextured->a("vertex"), 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    if (!mdl.mesh->texCoords.empty()) {
        glGenBuffers(1, &texCoordVBO);
        glBindBuffer(GL_ARRAY_BUFFER, texCoordVBO);
        glBufferData(GL_ARRAY_BUFFER, mdl.mesh->texCoords.size() * sizeof(float), mdl.mesh->texCoords.data(), GL_STATIC_DRAW);
        glEnableVertexAttribArray(spTextured->a("texCoord"));
        glVertexAttribPointer(spTextured->a("texCoord"), 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    }
    /*
    if (!mdl.mesh->normals.empty()) {
        glGenBuffers(1, &normalVBO);
        glBindBuffer(GL_ARRAY_BUFFER, normalVBO);
        glBufferData(GL_ARRAY_BUFFER, mdl.mesh->normals.size() * sizeof(float), mdl.mesh->normals.data(), GL_STATIC_DRAW);
        glEnableVertexAttribArray(spTextured->a("normal"));
        glVertexAttribPointer(spTextured->a("normal"), 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    }
    */

    if (mdl.diffuse) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, *(mdl.diffuse));
        glUniform1i(spTextured->u("tex"), 0);
    }

    glDrawElements(GL_TRIANGLES, mdl.mesh->indices.size(), GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);

    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    if (!mdl.mesh->texCoords.empty()) {
        glDeleteBuffers(1, &texCoordVBO);
    }
    if (!mdl.mesh->normals.empty()) {
        glDeleteBuffers(1, &normalVBO);
    }
    glDeleteVertexArrays(1, &VAO);
}






void renderer::draw_cube(const glm::mat4& M) {
	spLambert->use();
	glUniformMatrix4fv(spLambert->u("M"), 1, false, glm::value_ptr(M));
	glUniformMatrix4fv(spLambert->u("V"), 1, false, glm::value_ptr(renderer::V));
	glUniformMatrix4fv(spLambert->u("P"), 1, false, glm::value_ptr(renderer::P));
	Models::cube.drawSolid();
}

void renderer::draw_each_object(const std::vector<renderer::model*>& models) {
    for (const auto& model : models) {
        if (model->mesh) {
            renderer::render_model_tex(*model);
        }
        else {
            renderer::draw_cube(model->model_matrix);
        }
    }
}

void renderer::draw_scene(GLFWwindow* window) {
	//************Place any code here that draws something inside the window******************l
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Clear color and depth buffers

	//renderer::render_textured(glm::mat4(1.0f), myCubeVertices, myCubeTexCoords, myCubeVertexCount, tex);
	renderer::draw_each_object(renderer::all_models);
	ui_system::ui_visual::draw_ui();
	glfwSwapBuffers(window); //Copy back buffer to the front buffer

	// reseting mouse delta
	input_system::mouse_delta = glm::vec2(0.0, 0.0);

    
}









