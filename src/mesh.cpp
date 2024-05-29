#include "mesh.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <glm/glm.hpp>

std::map<std::string, renderer::mesh_ptr> renderer::mesh::mesh_map;

void renderer::mesh::calculate_tbn() {
    size_t vertexCount = indices.size();
    std::vector<std::vector<glm::vec4>> tbn;
    tbn.resize(vertexCount, std::vector<glm::vec4>(3, glm::vec4(0.0f)));

    for (size_t i = 0; i < indices.size(); i += 1) {
        int vertex_number = i % 3;
        int idx0;
        int idx1;
        int idx2;
        if (vertex_number == 0)
        {
            idx0 = indices[i];
            idx1 = indices[i + 1];
            idx2 = indices[i + 2];
        }
        else if (vertex_number == 1)
        {
            idx0 = indices[i];
            idx1 = indices[i - 1];
            idx2 = indices[i + 1];
        }
        else if (vertex_number == 2)
        {
            idx0 = indices[i];
            idx1 = indices[i - 1];
            idx2 = indices[i - 2];
        }

        glm::vec3 v0(vertices[idx0 * 3], vertices[idx0 * 3 + 1], vertices[idx0 * 3 + 2]);
        glm::vec3 v1(vertices[idx1 * 3], vertices[idx1 * 3 + 1], vertices[idx1 * 3 + 2]);
        glm::vec3 v2(vertices[idx2 * 3], vertices[idx2 * 3 + 1], vertices[idx2 * 3 + 2]);

        glm::vec2 uv0(texCoords[idx0 * 2], texCoords[idx0 * 2 + 1]);
        glm::vec2 uv1(texCoords[idx1 * 2], texCoords[idx1 * 2 + 1]);
        glm::vec2 uv2(texCoords[idx2 * 2], texCoords[idx2 * 2 + 1]);

        glm::vec3 deltaPos1 = v1 - v0;
        glm::vec3 deltaPos2 = v2 - v0;
        glm::vec2 deltaUV1 = uv1 - uv0;
        glm::vec2 deltaUV2 = uv2 - uv0;

        float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
        glm::vec3 tangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y) * r;
        glm::vec3 bitangent = (deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.x) * r;
        glm::vec3 normal = glm::normalize(glm::cross(tangent, bitangent));

        glm::vec4 tangent4(tangent, 0.0f);
        glm::vec4 bitangent4(bitangent, 0.0f);
        glm::vec4 normal4(normal, 0.0f);


        tbn[idx0][0] = tangent4;
        tbn[idx0][1] = bitangent4;
        tbn[idx0][2] = normal4;

    }
}

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

renderer::mesh_ptr renderer::mesh::load_mesh_from_file(const std::string& filename)
{
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Cannot open file: " << filename << std::endl;
        return nullptr;
    }

    std::vector<float> vertices_temp;
    std::vector<float> texCoords_temp;
    std::vector<float> normals_temp;
    std::vector<float> vertices;
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
            vertices_temp.push_back(x);
            vertices_temp.push_back(y);
            vertices_temp.push_back(z);
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
                vIndex[i] -= 1;
                indices.push_back(vIndex[i]);
                vertices.push_back(vertices_temp[vIndex[i] * 3]);
                vertices.push_back(vertices_temp[vIndex[i] * 3 + 1]);
                vertices.push_back(vertices_temp[vIndex[i] * 3 + 2]);
                vertices.push_back(1.0f);
                tIndex[i] -= 1;
                texCoords.push_back(texCoords_temp[tIndex[i] * 2]);
                texCoords.push_back(1.0f - texCoords_temp[tIndex[i] * 2 + 1]);
                nIndex[i] -= 1;
                normals.push_back(normals_temp[nIndex[i] * 3]);
                normals.push_back(normals_temp[nIndex[i] * 3 + 1]);
                normals.push_back(normals_temp[nIndex[i] * 3 + 2]);
                normals.push_back(0.0f);
            }
        }
    }

    return std::make_shared<renderer::mesh>(vertices, texCoords, normals, indices);
}
