#include "mesh.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <glm/glm.hpp>

std::map<std::string, renderer::mesh_ptr> renderer::mesh::mesh_map;

void renderer::mesh::calculate_tbn() {
    size_t vertexCount = vertices.size() / 3;
    std::vector<glm::vec3> tan1(vertexCount, glm::vec3(0.0f));
    std::vector<glm::vec3> tan2(vertexCount, glm::vec3(0.0f));
    std::vector<glm::vec3> norm(vertexCount, glm::vec3(0.0f));

    for (size_t i = 0; i < indices.size(); i += 3) {
        int i0 = indices[i];
        int i1 = indices[i + 1];
        int i2 = indices[i + 2];

        glm::vec3 v0(vertices[i0 * 3], vertices[i0 * 3 + 1], vertices[i0 * 3 + 2]);
        glm::vec3 v1(vertices[i1 * 3], vertices[i1 * 3 + 1], vertices[i1 * 3 + 2]);
        glm::vec3 v2(vertices[i2 * 3], vertices[i2 * 3 + 1], vertices[i2 * 3 + 2]);

        glm::vec2 uv0(texCoords[i0 * 2], texCoords[i0 * 2 + 1]);
        glm::vec2 uv1(texCoords[i1 * 2], texCoords[i1 * 2 + 1]);
        glm::vec2 uv2(texCoords[i2 * 2], texCoords[i2 * 2 + 1]);

        glm::vec3 deltaPos1 = v1 - v0;
        glm::vec3 deltaPos2 = v2 - v0;
        glm::vec2 deltaUV1 = uv1 - uv0;
        glm::vec2 deltaUV2 = uv2 - uv0;

        float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
        glm::vec3 tangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y) * r;
        glm::vec3 bitangent = (deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.x) * r;
        glm::vec3 normal = glm::normalize(glm::cross(deltaPos1, deltaPos2));

        tan1[i0] += tangent;
        tan1[i1] += tangent;
        tan1[i2] += tangent;

        tan2[i0] += bitangent;
        tan2[i1] += bitangent;
        tan2[i2] += bitangent;

        norm[i0] += normal;
        norm[i1] += normal;
        norm[i2] += normal;
    }

    for (size_t i = 0; i < vertexCount; ++i) {
        glm::vec3 n = glm::normalize(norm[i]);
        glm::vec3 t = glm::normalize(tan1[i]);

        glm::vec3 bitangent = glm::normalize(glm::cross(n, t));
        float handedness = (glm::dot(glm::cross(n, t), tan2[i]) < 0.0f) ? -1.0f : 1.0f;

        c1.push_back(t.x);
        c1.push_back(t.y);
        c1.push_back(t.z);

        c2.push_back(bitangent.x * handedness);
        c2.push_back(bitangent.y * handedness);
        c2.push_back(bitangent.z * handedness);

        c3.push_back(n.x);
        c3.push_back(n.y);
        c3.push_back(n.z);
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
