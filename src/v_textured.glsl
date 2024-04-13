#version 330

//Uniform variables
uniform mat4 P;
uniform mat4 V;
uniform mat4 M;



//Attributes
layout (location=0) in vec4 vertex; //vertex coordinates in model space
layout (location=2) in vec2 texCoord; //texturing coordinates


//varying variables
out vec2 i_tc;

void main(void) {
    gl_Position=P*V*M*vertex;
    i_tc=texCoord;
}
