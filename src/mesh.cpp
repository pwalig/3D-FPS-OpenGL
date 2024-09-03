#include "mesh.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <glm/glm.hpp>
#include <thread>
#include "meshReader.h"
#include <key_bind.h>
#include <GLFW/glfw3.h>

std::map<std::string, renderer::mesh*> renderer::mesh::mesh_map;

#ifdef _DEBUG
input_system::key_bind* mesh_map_info_kb;
#endif 

renderer::mesh::mesh(const std::string& filename_) : filename(filename_), refs(0), delete_on_0_refs(true)
{
    if (filename.substr(filename.size() - 5) == ".mesh") this->load_mesh_from_mesh_file(filename);
    else this->load_mesh_from_obj_file(filename);

#ifdef _DEBUG
    printf("loaded mesh: %s\n", filename.c_str());
#endif 
}

renderer::mesh::~mesh()
{
#ifdef _DEBUG
    printf("deleted mesh: %s\n", filename.c_str());
#endif 
}

void renderer::mesh::set_delete_on_0_refs(const bool& del)
{
    this->delete_on_0_refs = del;

    if (this->refs <= 0 && this->delete_on_0_refs) {
        erase_resource_from_map(this->filename);
        delete this;
    }
}

void renderer::mesh::init()
{    
    std::thread t1(pre_load, "../assets/models/Tower.mesh");
    std::thread t2(pre_load, "../assets/models/snakeguy3.mesh");
    std::thread t3(pre_load, "../assets/models/demon.mesh");
    std::thread t4(pre_load, "../assets/models/Ghost.mesh");
    std::thread t5(pre_load, "../assets/models/monster.mesh");
    std::thread t6(pre_load, "../assets/models/colliders/sphere_collider.mesh");
    t1.join();
    t2.join();
    t3.join();
    t4.join();
    t5.join();
    t6.join();

#ifdef _DEBUG
    mesh_map_info_kb = new input_system::key_bind([]() { renderer::mesh::print_mesh_map_info(); }, GLFW_KEY_F4, GLFW_PRESS);
#endif 
}

void renderer::mesh::pre_load(const std::string& filename)
{
    renderer::mesh_ptr m(filename);
    m->delete_on_0_refs = false;
}

void renderer::mesh::free_all()
{
    for (std::pair<const std::string, renderer::mesh*> tex : mesh_map) {
        delete (tex.second);
    }
    mesh_map.clear();
#ifdef _DEBUG
    delete mesh_map_info_kb;
#endif 
}

void renderer::mesh::print_mesh_map_info()
{
    printf("mesh map info:\nname : refs : delete on 0 refs\n");
    for (std::pair<const std::string, renderer::mesh*> m : mesh_map) {
        printf("%s : %d : %d\n", m.first.c_str(), (int)(m.second->refs), m.second->delete_on_0_refs ? 1 : 0);
    }
}

void renderer::mesh::erase_resource_from_map(const std::string& filename)
{
    std::map<std::string, renderer::mesh*>::iterator it = mesh_map.find(filename);
    if (it != mesh_map.end()) mesh_map.erase(it);
}

void renderer::mesh::load_mesh_from_obj_file(const std::string& filename)
{
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Cannot open file: " << filename << std::endl;
        return;
    }

    std::vector<glm::vec3> vertices_temp;
    std::vector<glm::vec2> texCoords_temp;
    std::vector<glm::vec3> normals_temp;
    std::vector<glm::vec3> tangents_temp;
    std::vector<glm::vec3> bitangents_temp;
    std::vector<obj_face> obj_faces;
    std::string line;

    while (std::getline(file, line)) {
        std::istringstream ss(line);
        std::string type;
        ss >> type;

        if (type == "v") {
            float x, y, z;
            ss >> x >> y >> z;
            vertices_temp.push_back(glm::vec3(x, y, z));
        }
        else if (type == "vt") {
            float u, v;
            ss >> u >> v;
            texCoords_temp.push_back(glm::vec2(u, v));
        }
        else if (type == "vn") {
            float nx, ny, nz;
            ss >> nx >> ny >> nz;
            normals_temp.push_back(glm::vec3(nx, ny, nz));
            tangents_temp.push_back(glm::vec3(0.0f)); // just to reserve space
            bitangents_temp.push_back(glm::vec3(0.0f)); // just to reserve space
        }
        else if (type == "f") {
            obj_face of(ss);
            obj_faces.push_back(of);

            // tbn
            glm::vec3 deltaPos1 = vertices_temp[of.vIndex[1]] - vertices_temp[of.vIndex[0]];
            glm::vec3 deltaPos2 = vertices_temp[of.vIndex[2]] - vertices_temp[of.vIndex[0]];
            glm::vec2 deltaUV1 = texCoords_temp[of.tIndex[1]] - texCoords_temp[of.tIndex[0]];
            glm::vec2 deltaUV2 = texCoords_temp[of.tIndex[2]] - texCoords_temp[of.tIndex[0]];

            float denom = deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x;
            if (denom == 0.0f) {
                continue; // Pomijamy ten trójk¹t, jeœli mianownik jest zbyt ma³y
            }

            glm::vec3 tangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y) / denom;
            glm::vec3 bitangent = (deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.x) / denom;

            tangents_temp[of.nIndex[0]] += tangent;
            tangents_temp[of.nIndex[1]] += tangent;
            tangents_temp[of.nIndex[2]] += tangent;

            bitangents_temp[of.nIndex[0]] += bitangent;
            bitangents_temp[of.nIndex[1]] += bitangent;
            bitangents_temp[of.nIndex[2]] += bitangent;
        }
    }
    file.close();
    this->indices.clear();
    this->vertices.clear();
    this->normals.clear();
    this->texCoords.clear();
    this->tangents.clear();
    this->bitangents.clear();
    std::thread t1([&obj_faces, this, &vertices_temp, &texCoords_temp, &normals_temp]() {
        for (const obj_face& of : obj_faces) {
            for (int i = 0; i < 3; ++i) {
                // verts
                this->vertices.push_back(vertices_temp[of.vIndex[i]].x);
                this->vertices.push_back(vertices_temp[of.vIndex[i]].y);
                this->vertices.push_back(vertices_temp[of.vIndex[i]].z);
                this->vertices.push_back(1.0f);

                // texCoords
                this->texCoords.push_back(texCoords_temp[of.tIndex[i]].x);
                this->texCoords.push_back(1.0f - texCoords_temp[of.tIndex[i]].y);

                // normals
                this->normals.push_back(normals_temp[of.nIndex[i]].x);
                this->normals.push_back(normals_temp[of.nIndex[i]].y);
                this->normals.push_back(normals_temp[of.nIndex[i]].z);
            }
        }
    });
    std::thread t2([&obj_faces, this, &tangents_temp, &bitangents_temp, &normals_temp]() {
        for (const obj_face& of : obj_faces) {
            for (int i = 0; i < 3; ++i) {
                // tbn
                tangents_temp[of.nIndex[i]] = glm::normalize(tangents_temp[of.nIndex[i]]);
                bitangents_temp[of.nIndex[i]] = glm::normalize(bitangents_temp[of.nIndex[i]]);

                this->tangents.push_back(tangents_temp[of.nIndex[i]].x);
                this->tangents.push_back(tangents_temp[of.nIndex[i]].y);
                this->tangents.push_back(tangents_temp[of.nIndex[i]].z);

                this->bitangents.push_back(bitangents_temp[of.nIndex[i]].x);
                this->bitangents.push_back(bitangents_temp[of.nIndex[i]].y);
                this->bitangents.push_back(bitangents_temp[of.nIndex[i]].z);
            }
        }
    });
    t1.join();
    t2.join();
}

void renderer::mesh::load_mesh_from_mesh_file(const std::string& filename)
{
    std::ifstream file(filename, std::ios::in | std::ios::binary);
    if (!file) {
        std::cerr << "Cannot open file: " << filename << std::endl;
        return;
    }

/* expected format:

fielde i0 i1 i2
fielde vx vy vz f1.0
fielde 0x 0y
fielde nx ny nz
fielde tx ty tz
fielde bx by bz
*/
    mesh_reader::readBuffer<unsigned int, unsigned int>(file, this->indices);
    mesh_reader::readBuffer<float, unsigned int>(file, this->vertices);
    mesh_reader::readBuffer<float, unsigned int>(file, this->texCoords);
    mesh_reader::readBuffer<float, unsigned int>(file, this->normals);
    mesh_reader::readBuffer<float, unsigned int>(file, this->tangents);
    mesh_reader::readBuffer<float, unsigned int>(file, this->bitangents);

    file.close();
}

renderer::mesh::obj_face::obj_face(std::istringstream& ss)
{
    std::string vertex1, vertex2, vertex3;

    ss >> vertex1 >> vertex2 >> vertex3;

    std::replace(vertex1.begin(), vertex1.end(), '/', ' ');
    std::replace(vertex2.begin(), vertex2.end(), '/', ' ');
    std::replace(vertex3.begin(), vertex3.end(), '/', ' ');

    std::istringstream v1(vertex1);
    std::istringstream v2(vertex2);
    std::istringstream v3(vertex3);

    v1 >> this->vIndex[0] >> this->tIndex[0] >> this->nIndex[0];
    v2 >> this->vIndex[1] >> this->tIndex[1] >> this->nIndex[1];
    v3 >> this->vIndex[2] >> this->tIndex[2] >> this->nIndex[2];

    for (int i = 0; i < 3; ++i) {
        this->vIndex[i] -= 1;
        this->tIndex[i] -= 1;
        this->nIndex[i] -= 1;
    }
}

renderer::mesh_ptr::mesh_ptr(const std::string& filename)
{
    auto it = renderer::mesh::mesh_map.find(filename);
    if (it != renderer::mesh::mesh_map.end()) {
        this->mesh = it->second;
    }
    else {
        mesh::mesh_map[filename] = new renderer::mesh(filename);
        this->mesh = mesh::mesh_map[filename];
    }
    this->mesh->refs++;
}

renderer::mesh_ptr::mesh_ptr(const mesh_ptr& other) : mesh(other.mesh)
{
    this->mesh->refs++;
}

renderer::mesh_ptr::mesh_ptr(mesh_ptr&& other) noexcept : mesh(other.mesh)
{
    this->mesh->refs++;
}

renderer::mesh_ptr& renderer::mesh_ptr::operator=(const mesh_ptr& other)
{
    if (this != &other) {
        this->mesh = other.mesh;
        this->mesh->refs++;
    }
    return *this;
}

renderer::mesh_ptr& renderer::mesh_ptr::operator=(mesh_ptr&& other) noexcept
{
    if (this != &other) {
        this->mesh = other.mesh;
        this->mesh->refs++;
    }
    return *this;
}

renderer::mesh* renderer::mesh_ptr::operator->()
{
    return this->mesh;
}

renderer::mesh& renderer::mesh_ptr::operator*()
{
    return *(this->mesh);
}

renderer::mesh* renderer::mesh_ptr::get()
{
    return this->mesh;
}

renderer::mesh_ptr::~mesh_ptr()
{
    this->mesh->refs--;
    if (this->mesh->refs <= 0 && this->mesh->delete_on_0_refs) {
        mesh::erase_resource_from_map(this->mesh->filename);
        delete this->mesh;
        this->mesh = nullptr;
    }
}
