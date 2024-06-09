#version 330
#define MAX_LIGHTS 32

// texture maps
uniform sampler2D diffuse_map;
uniform sampler2D normal_map;
uniform sampler2D height_map;
uniform sampler2D specular_map;

// lights data
uniform int lights;
uniform vec3[MAX_LIGHTS] light_positions;
uniform vec3[MAX_LIGHTS] light_colors;

uniform mat4 M;

out vec4 pixelColor; //Zmienna wyjsciowa fragment shadera. Zapisuje sie do niej ostateczny (prawie) kolor piksela

in mat4 invTBN;
in vec4 v;
in vec4 vert;
in vec2 iTexCoord;

vec2 parallaxTextureCoords(vec4 v, vec2 t, float h, float s) {
	// increments
	vec2 ti = -h * v.xy / s;
	float hi = -1 / s;

	// current values
	vec2 tc = t;
	float hc = 1;
	
	float ht = texture(height_map, tc).r; // height in texture

	if (v.z <= 0) discard; // viewer under face
	while (hc > ht) { // while you are above surface
		tc += ti; // make a step on a texture

		// when out of texture repeat
		if (tc.x < 0) tc.x += 1;
		if (tc.y < 0) tc.y += 1;
		if (tc.x > 1) tc.x -= 1;
		if (tc.y > 1) tc.y -= 1;

		hc += hi; // make a step downwards
		ht = texture(height_map, tc).r; //new height from texture
	}

	// linear interpolation
	vec2 tco = tc - ti;
	float hco = hc - hi;
	float hto = texture(height_map, tco).r;

	float x = (hco - hto) / (hco - hto - (hc - ht));

	return (1 - x) * tco + x * tc;
}

void main(void) {
	vec3 kd = vec3(0, 0, 0);
	vec3 ks = vec3(0, 0, 0);
	vec4 nv = normalize(v);

	vec2 pTexCoords = parallaxTextureCoords(nv, iTexCoord, 0.01, 10); // paralax texture coordinates
	vec4 diffuse = texture(diffuse_map, pTexCoords);
	vec4 specular = texture(specular_map, pTexCoords);

	for (int i = 0; i < lights; ++i) {
		vec4 l = normalize(invTBN * inverse(M) * vec4(light_positions[i], 1) - (invTBN * vert)); //wektor do światła w przestrzeni tbn
		float r = length(vec4(light_positions[i], 1) - (M * vert)); // distance to light

		vec4 n = normalize(vec4(2 * texture(normal_map, pTexCoords).xyz - 1, 0)); // normal vector in tbn space

		vec4 refl = reflect(-l, n); // Wektor odbity in tbn space
		float rv = pow(clamp(dot(refl, nv), 0, 1), 25);
		float nl = clamp(dot(n, l), 0, 1);
		
		ks += vec3(1,1,1) * rv * light_colors[i] / (r * r); // reflected / specular light
		kd += diffuse.rgb * nl * light_colors[i] / (r * r); // refracted / diffused light
	}

	pixelColor = vec4(kd.rgb + ks.rgb, diffuse.a);
}
