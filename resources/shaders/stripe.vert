#version 410

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normalIn;
layout (location = 2) in vec2 texCoord;

uniform mat4 translate;
uniform mat4 rotate;
uniform mat4 scale;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 eyePosition;

out vec3 normal;
out vec3 normalLocal;

void main() {
	mat4 model = translate * rotate * scale;
	mat4 mvp = projection * view * model;
	gl_Position = mvp * vec4(position, 1.0f);

	vec4 normal4 = vec4(normalIn, 1.0f);
	vec4 wNormal = rotate * normal4;
	normal = vec3(wNormal);
	normalLocal = normalIn;
}
