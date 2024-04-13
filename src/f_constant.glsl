#version 330


out vec4 pixelColor; //Output variable of the fragment shader. (Almost) final pixel color.


uniform vec4 color=vec4(1,1,1,1);

void main(void) {
	pixelColor=color;
}
