#version 330

//Uniform variables
uniform mat4 P;
uniform mat4 V;
uniform mat4 M;

//Attributes
layout (location=0) in vec4 vertex; //vertex coordinates in model space
layout (location=3) in vec4 color; //vertex color

out vec4 i_c;

void main(void) {
    i_c=color;
    gl_Position=P*V*M*vertex;
}
