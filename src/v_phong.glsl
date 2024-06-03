#version 330

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
out vec4 l;
out vec4 v;
out vec2 iTexCoord;


void main(void) {
    mat4 invTBN = mat4(c1, c2, c3, vec4(0, 0, 0, 1));
    vec4 lp = inverse(V) * vec4(0, 0, 0, 1); //przestrzeñ œwiata

    l = normalize(invTBN * inverse(M) * lp - invTBN * vertex); //wektor do œwiat³a w przestrzeni tbn
    v = normalize(invTBN * inverse(V * M) * vec4(0, 0, 0, 1) - invTBN * vertex); // tbn space

    iTexCoord = texture_coordinates;

    gl_Position = P * V * M * vertex;
}
