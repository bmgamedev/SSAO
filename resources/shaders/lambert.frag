#version 410

/*layout (location = 0) in vec3 fPosition;

layout (location = 0) out vec4 fColour;

in vec3 normal;*/

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_normal;
layout (location = 2) in vec2 in_uv;

out vec4 out_colour;

struct DirectionalLight {
	vec3 colour;
	vec3 direction;
};

//This shader applies a Lambert shading model and uses a directional light as single light source.
void main() {
	/*// directional light properties
	DirectionalLight light;
	light.colour = vec3(0.5F, 0.4F, 0.3F);
	//light.direction = vec3(-2.0F, 0.5F, 3.7F);
	light.direction = vec3(1.0F, 2.0F, 3.0F);

	// ambient
	float ambientIntensity = 0.5F;
	vec3 ambient = light.colour * ambientIntensity;

	// diffuse
	float diffuseIntensity = 1.0F;
	vec3 diffuse = 0.2F * max(dot(light.direction, normal), 0.0F) * light.colour * diffuseIntensity;

	// final colour
	fColour = vec4(ambient + diffuse, 1.0F);*/

	DirectionalLight light;
	light.colour    = normalize(vec3(0.5f, 0.4f, 0.3f));
	light.direction = normalize(vec3(1.0f, 2.0f, 3.0f));

	// Ambient
	float ambientIntensity = 0.5f;
	vec3 ambient = light.colour * ambientIntensity;

	// Diffuse
	float diffuseIntensity = 1.0f;
	vec3 diffuse = 0.2f	* max(dot(light.direction, in_normal), 0.0f) * light.colour	* diffuseIntensity;

	// Final colour
	out_colour = vec4(ambient + diffuse, 1.0f);
}