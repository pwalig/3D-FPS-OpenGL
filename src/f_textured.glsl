#version 330


uniform sampler2D tex;

out vec4 pixelColor; //Output variable of the fragment shader. (Almost) final pixel color.

//Varying variables
in vec2 i_tc;

void main(void) {
	pixelColor=texture(tex,i_tc);
}
