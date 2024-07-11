#include "mesh.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <glm/glm.hpp>
#include <thread>
#include "meshReader.h"
#include "debug_defines.h"

std::map<std::string, renderer::mesh_ptr> renderer::mesh::mesh_map;

renderer::mesh_ptr renderer::mesh::get_mesh(const std::string& filename)
{
    auto it = renderer::mesh::mesh_map.find(filename);
    if (it != renderer::mesh::mesh_map.end()) {
        return it->second;
    }
#ifdef DEBUG
    printf("load %s\n", filename.c_str());
#endif // DEBUG
    renderer::mesh_ptr mesh;
    if (filename.substr(filename.size()-5) == ".mesh") mesh = load_mesh_from_mesh_file(filename);
    else mesh = load_mesh_from_file(filename);
    if (mesh) {
        renderer::mesh::mesh_map[filename] = mesh;
    }
    return mesh;
}

void renderer::mesh::init()
{    
    std::thread t1(get_mesh, "../assets/models/Tower.mesh");
    std::thread t2(get_mesh, "../assets/models/snakeguy3.mesh");
    std::thread t3(get_mesh, "../assets/models/demon.mesh");
    std::thread t4(get_mesh, "../assets/models/Ghost.mesh");
    std::thread t5(get_mesh, "../assets/models/monster.mesh");
    t1.join();
    t2.join();
    t3.join();
    t4.join();
    t5.join();
}

renderer::mesh_ptr renderer::mesh::load_mesh_from_file(const std::string& filename)
{
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Cannot open file: " << filename << std::endl;
        return nullptr;
    }

    mesh_ptr m = std::make_shared<renderer::mesh>();

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
    mesh* mp = m.get();
    std::thread t1([&obj_faces, mp, &vertices_temp, &texCoords_temp, &normals_temp]() {
        for (const obj_face& of : obj_faces) {
            for (int i = 0; i < 3; ++i) {
                // verts
                mp->vertices.push_back(vertices_temp[of.vIndex[i]].x);
                mp->vertices.push_back(vertices_temp[of.vIndex[i]].y);
                mp->vertices.push_back(vertices_temp[of.vIndex[i]].z);
                mp->vertices.push_back(1.0f);

                // texCoords
                mp->texCoords.push_back(texCoords_temp[of.tIndex[i]].x);
                mp->texCoords.push_back(1.0f - texCoords_temp[of.tIndex[i]].y);

                // normals
                mp->normals.push_back(normals_temp[of.nIndex[i]].x);
                mp->normals.push_back(normals_temp[of.nIndex[i]].y);
                mp->normals.push_back(normals_temp[of.nIndex[i]].z);
            }
        }
    });
    std::thread t2([&obj_faces, mp, &tangents_temp, &bitangents_temp, &normals_temp]() {
        for (const obj_face& of : obj_faces) {
            for (int i = 0; i < 3; ++i) {
                // tbn
                tangents_temp[of.nIndex[i]] = glm::normalize(tangents_temp[of.nIndex[i]]);
                bitangents_temp[of.nIndex[i]] = glm::normalize(bitangents_temp[of.nIndex[i]]);

                mp->tangents.push_back(tangents_temp[of.nIndex[i]].x);
                mp->tangents.push_back(tangents_temp[of.nIndex[i]].y);
                mp->tangents.push_back(tangents_temp[of.nIndex[i]].z);

                mp->bitangents.push_back(bitangents_temp[of.nIndex[i]].x);
                mp->bitangents.push_back(bitangents_temp[of.nIndex[i]].y);
                mp->bitangents.push_back(bitangents_temp[of.nIndex[i]].z);
            }
        }
    });
    t1.join();
    t2.join();
    return m;
}

renderer::mesh_ptr renderer::mesh::load_mesh_from_mesh_file(const std::string& filename)
{
    std::ifstream file(filename, std::ios::in | std::ios::binary);
    if (!file) {
        std::cerr << "Cannot open file: " << filename << std::endl;
        return nullptr;
    }
    mesh m;
/* expected format:

fielde i0 i1 i2
fielde vx vy vz f1.0
fielde 0x 0y
fielde nx ny nz
fielde tx ty tz
fielde bx by bz
*/
    mesh_reader::readBuffer<unsigned int, unsigned int>(file, m.indices);
    mesh_reader::readBuffer<float, unsigned int>(file, m.vertices);
    mesh_reader::readBuffer<float, unsigned int>(file, m.texCoords);
    mesh_reader::readBuffer<float, unsigned int>(file, m.normals);
    mesh_reader::readBuffer<float, unsigned int>(file, m.tangents);
    mesh_reader::readBuffer<float, unsigned int>(file, m.bitangents);

    file.close();
    return std::make_shared<renderer::mesh>(m);
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
