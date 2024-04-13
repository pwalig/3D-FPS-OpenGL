#version 330

//Uniform variables
uniform mat4 P;
uniform mat4 V;
uniform mat4 M;

//Attributes
layout (location=0) in vec4 vertex; //vertex coordinates in model space


void main(void) {
    gl_Position=P*V*M*vertex;
}
