#version 330

//Zmienne jednorodne
uniform mat4 P;
uniform mat4 V;
uniform mat4 M;

//Atrybuty
in vec4 vertex; //wspolrzedne wierzcholka w przestrzeni modelu
in vec2 texture_coordinates;
in vec3 normal;
in vec3 tangent;
in vec3 bitangent;

//Zmienne interpolowane
out mat4 invTBN;
out vec4 v;
out vec4 vert;
out vec2 iTexCoord;


void main(void) {
    invTBN = mat4(
        tangent.x, bitangent.x, normal.x, 0,
        tangent.y, bitangent.y, normal.y, 0,
        tangent.z, bitangent.z, normal.z, 0,
        0, 0, 0, 1
    );
    v = normalize(invTBN * inverse(V * M) * vec4(0, 0, 0, 1) - (invTBN * vertex)); // to viewer vector in tbn space
    iTexCoord = texture_coordinates;
    vert = vertex;
    
    gl_Position=P*V*M*vertex;
}
