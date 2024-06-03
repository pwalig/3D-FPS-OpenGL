#version 330
#define MAX_LIGHTS 16

//Zmienne jednorodne
uniform mat4 P;
uniform mat4 V;
uniform mat4 M;

//Atrybuty
in vec4 vertex; //wspolrzedne wierzcholka w przestrzeni modelu
in vec2 texture_coordinates;
in vec4 c1;
in vec4 c2;
in vec4 c3;

//Zmienne interpolowane
out mat4 invTBN;
out vec4 v;
out vec4 vert;
out vec2 iTexCoord;


void main(void) {
    invTBN = mat4(c1, c2, c3, vec4(0, 0, 0, 1));
    v = normalize(invTBN * inverse(V * M) * vec4(0, 0, 0, 1) - (invTBN * vertex)); // to viewer vector in tbn space
    iTexCoord = texture_coordinates;
    vert = vertex;
    
    gl_Position=P*V*M*vertex;
}
