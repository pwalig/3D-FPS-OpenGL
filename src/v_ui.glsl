#version 330

//Uniform variables
uniform mat4 P;
uniform mat4 M;

//Attributes
in vec4 vertex; //vertex coordinates in model space
in vec2 texCoord; //texturing coordinates

out vec2 i_tc;

void main(void) {
    gl_Position = P * M * vertex;
    i_tc = texCoord;
}
