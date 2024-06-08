#include "mesh.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <glm/glm.hpp>
#include <thread>

std::map<std::string, renderer::mesh_ptr> renderer::mesh::mesh_map;

renderer::mesh_ptr renderer::mesh::get_mesh(const std::string& filename)
{
    auto it = renderer::mesh::mesh_map.find(filename);
    if (it != renderer::mesh::mesh_map.end()) {
        return it->second;
    }

    renderer::mesh_ptr mesh = load_mesh_from_file(filename);
    if (mesh) {
        renderer::mesh::mesh_map[filename] = mesh;
    }
    return mesh;
}

void renderer::mesh::init()
{
    std::thread t1(get_mesh, "../assets/models/Tower.obj");
    std::thread t2(get_mesh, "../assets/models/snakeguy3.obj");
    std::thread t3(get_mesh, "../assets/models/demon.obj");
    std::thread t4(get_mesh, "../assets/models/Ghost.obj");
    std::thread t5(get_mesh, "../assets/models/monster.obj");
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
            std::string vertex1, vertex2, vertex3;
            int vIndex[3], tIndex[3], nIndex[3];

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
                vIndex[i] -= 1;
                tIndex[i] -= 1;
                nIndex[i] -= 1;
            }

            // tbn
            glm::vec3 deltaPos1 = vertices_temp[vIndex[1]] - vertices_temp[vIndex[0]];
            glm::vec3 deltaPos2 = vertices_temp[vIndex[2]] - vertices_temp[vIndex[0]];
            glm::vec2 deltaUV1 = texCoords_temp[tIndex[1]] - texCoords_temp[tIndex[0]];
            glm::vec2 deltaUV2 = texCoords_temp[tIndex[2]] - texCoords_temp[tIndex[0]];

            float denom = deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x;
            if (denom == 0.0f) {
                continue; // Pomijamy ten trójk¹t, jeœli mianownik jest zbyt ma³y
            }

            glm::vec3 tangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y) / denom;
            glm::vec3 bitangent = (deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.x) / denom;

            tangents_temp[nIndex[0]] += tangent;
            tangents_temp[nIndex[1]] += tangent;
            tangents_temp[nIndex[2]] += tangent;

            bitangents_temp[nIndex[0]] += bitangent;
            bitangents_temp[nIndex[1]] += bitangent;
            bitangents_temp[nIndex[2]] += bitangent;
        }
    }
    file.close();
    file.open(filename);
    while (std::getline(file, line)) {
        std::istringstream ss(line);
        std::string type;
        ss >> type;
        if (type == "f") {
            std::string vertex1, vertex2, vertex3;
            int vIndex[3], tIndex[3], nIndex[3];

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
                vIndex[i] -= 1;
                tIndex[i] -= 1;
                nIndex[i] -= 1;

                // verts
                m.get()->vertices.push_back(vertices_temp[vIndex[i]].x);
                m.get()->vertices.push_back(vertices_temp[vIndex[i]].y);
                m.get()->vertices.push_back(vertices_temp[vIndex[i]].z);
                m.get()->vertices.push_back(1.0f);

                // texCoords
                m.get()->texCoords.push_back(texCoords_temp[tIndex[i]].x);
                m.get()->texCoords.push_back(1.0f - texCoords_temp[tIndex[i]].y);

                // normals
                m.get()->normals.push_back(normals_temp[nIndex[i]].x);
                m.get()->normals.push_back(normals_temp[nIndex[i]].y);
                m.get()->normals.push_back(normals_temp[nIndex[i]].z);
                m.get()->normals.push_back(0.0f);

                // tbn
                tangents_temp[nIndex[i]] = glm::normalize(tangents_temp[nIndex[i]]);
                bitangents_temp[nIndex[i]] = glm::normalize(bitangents_temp[nIndex[i]]);

                m.get()->c1.push_back(tangents_temp[nIndex[i]].x);
                m.get()->c2.push_back(tangents_temp[nIndex[i]].y);
                m.get()->c3.push_back(tangents_temp[nIndex[i]].z);

                m.get()->c1.push_back(bitangents_temp[nIndex[i]].x);
                m.get()->c2.push_back(bitangents_temp[nIndex[i]].y);
                m.get()->c3.push_back(bitangents_temp[nIndex[i]].z);

                m.get()->c1.push_back(normals_temp[nIndex[i]].x);
                m.get()->c2.push_back(normals_temp[nIndex[i]].y);
                m.get()->c3.push_back(normals_temp[nIndex[i]].z);

                m.get()->c1.push_back(0.0f);
                m.get()->c2.push_back(0.0f);
                m.get()->c3.push_back(0.0f);
            }
        }
    }
    file.close();
    return m;
}
