#version 330

//Uniform variables
uniform mat4 P;
uniform mat4 M;
uniform mat4 A; // anchor matrix
uniform vec3 pivot;

//Attributes
in vec4 vertex; //vertex coordinates in model space
in vec2 texCoord; //texturing coordinates

out vec2 i_tc;

void main(void) {
    gl_Position = P * A * M * (vertex - vec4(pivot, 0));
    i_tc = texCoord;
}
