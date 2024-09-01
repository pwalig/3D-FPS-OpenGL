#version 330
#define MAX_LIGHTS 32

// texture maps
uniform sampler2D albedo_map;
uniform sampler2D normal_map;
uniform sampler2D height_map;
uniform sampler2D data_map;

// adjustables
uniform vec3 albedo_;
uniform float roughness_;
uniform float metallic_;
uniform float ao_;
uniform vec3 ambient_;

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

const float PI = 3.14159265359;

vec2 parallaxTextureCoords(vec3 v, vec2 t, float h, float s) {
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
		ht = texture(height_map, tc).r; // get new height from texture
	}

	// linear interpolation
	vec2 tco = tc - ti;
	float hco = hc - hi;
	float hto = texture(height_map, tco).r;

	float x = (hco - hto) / (hco - hto - (hc - ht));

	return (1 - x) * tco + x * tc;
}

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
	return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
	float a = roughness * roughness;
	float a2 = a * a;
	float NdotH = max(dot(N, H), 0.0);
	float NdotH2 = NdotH * NdotH;

	float num = a2;
	float denom = (NdotH2 * (a2 - 1.0) + 1.0);
	denom = PI * denom * denom;

	return num / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
	float r = (roughness + 1.0);
	float k = (r * r) / 8.0;

	float num = NdotV;
	float denom = NdotV * (1.0 - k) + k;

	return num / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
	float NdotV = max(dot(N, V), 0.0);
	float NdotL = max(dot(N, L), 0.0);
	float ggx2 = GeometrySchlickGGX(NdotV, roughness);
	float ggx1 = GeometrySchlickGGX(NdotL, roughness);

	return ggx1 * ggx2;
}

void main(void) {
	vec3 V = normalize(v.xyz); // to viewer vector in tbn space
	vec2 pTexCoords = parallaxTextureCoords(V, iTexCoord, 0.01, 10); // paralax texture coordinates
	vec3 N = normalize(2 * texture(normal_map, pTexCoords).xyz - 1); // normal vector in tbn space

	vec3 albedo = pow(texture(albedo_map, pTexCoords).rgb, vec3(2.2)) * albedo_;
	float alpha = texture(albedo_map, pTexCoords).a;
	vec4 data = texture(data_map, pTexCoords);
	float roughness = data.r * roughness_;
	float metallic = data.g * metallic_;
	float ao = 1.0 - ((1.0 - data.b) * ao_);

	vec3 F0 = vec3(0.04);
	F0 = mix(F0, albedo, metallic);

	// reflectance equation
	vec3 Lo = vec3(0.0);

	for (int i = 0; i < lights && i < MAX_LIGHTS; ++i) {
		vec3 L = normalize((invTBN * inverse(M) * vec4(light_positions[i], 1) - (invTBN * vert)).xyz); // to light vector in tbn space
		vec3 H = normalize(V + L); // halfway vector in tbn space

		float r = length(light_positions[i] - (M * vert).xyz); // distance to light
		vec3 radiance = light_colors[i] / (r * r);

		// cook-torrance brdf
		float NDF = DistributionGGX(N, H, roughness);
		float G = GeometrySmith(N, V, L, roughness);
		vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);

		vec3 kS = F;
		vec3 kD = vec3(1.0) - kS;
		kD *= 1.0 - metallic;

		vec3 numerator = NDF * G * F;
		float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
		vec3 specular = numerator / denominator;

		// add to outgoing radiance Lo
		float NdotL = max(dot(N, L), 0.0);
		Lo += (kD * albedo / PI + specular) * radiance * NdotL;
	}

	vec3 ambient = ambient_ * albedo * ao;
	vec3 color = Lo + ambient;

	color = color / (color + vec3(1.0));
	color = pow(color, vec3(1.0 / 2.2));

	pixelColor = vec4(color, alpha);
}
