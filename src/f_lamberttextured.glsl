#version 330


uniform sampler2D tex;

out vec4 pixelColor; //Output variable of the fragment shader. (Almost) final pixel color.

//Varying variables
in float i_nl;
in vec2 i_tc;

void main(void) {
    vec4 color=texture(tex,i_tc);
	pixelColor=vec4(color.rgb*i_nl,color.a);
}
