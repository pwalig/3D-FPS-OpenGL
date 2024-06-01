#version 330

uniform sampler2D diffuse_map;
uniform sampler2D normal_map;
uniform sampler2D height_map;
uniform sampler2D specular_map;

out vec4 pixelColor; //Zmienna wyjsciowa fragment shadera. Zapisuje sie do niej ostateczny (prawie) kolor piksela

in vec4 l;
in vec4 v;
in vec2 iTexCoord;

vec2 parallaxTextureCoords(vec4 v, vec2 t, float h, float s) {
	vec2 ti = -h * v.xy / s;
	float hi = -1 / s; //increments

	vec2 tc = t;
	float hc = 1; //current values
	
	float ht = texture(height_map, tc).r; //height in texture

	if (v.z <= 0) discard; //viewer under face
	while (hc > ht) { //while you are above surface
		tc += ti; //make a step on a texture
		//Do not go out of the texture
		if (tc.x < 0) tc.x += 1;
		if (tc.y < 0) tc.y += 1;
		if (tc.x > 1) tc.x -= 1;
		if (tc.y > 1) tc.y -= 1;
		//if (tc.x < 0 || tc.x > 1 || tc.y < 0 || tc.y > 1) discard;
		hc += hi; //make a step downwards
		//new height from texture
		ht = texture(height_map, tc).r;
	}

	vec2 tco = tc - ti;
	float hco = hc - hi;
	float hto = texture(height_map, tco).r;

	float x = (hco - hto) / (hco - hto - (hc - ht));

	return (1 - x) * tco + x * tc;
}

void main(void) {

	//Znormalizowane interpolowane wektory
	vec4 mv = normalize(v);
	vec2 pTexCoords = parallaxTextureCoords(mv, iTexCoord, 0.01, 10);
	vec4 ml = normalize(l);
	vec4 mn = normalize(vec4(2 * texture(normal_map, pTexCoords).xyz - 1, 0));
	//Wektor odbity
	vec4 mr = reflect(-ml, mn);

	//Parametry powierzchni
	vec4 kd = texture(diffuse_map, pTexCoords);
	vec4 ks = texture(specular_map, pTexCoords);

	//Obliczenie modelu oświetlenia
	float nl = clamp(dot(mn, ml), 0, 1);
	float rv = pow(clamp(dot(mr, mv), 0, 1),25);
	pixelColor = vec4(kd.rgb * nl, kd.a) +vec4(ks.rgb * rv, 0);
}
