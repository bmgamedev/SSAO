#version 410

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_normal;
layout (location = 2) in vec2 in_uv;

layout (location = 0) out vec3 out_position;
layout (location = 1) out vec3 out_normal;
layout (location = 2) out vec2 out_uv;

uniform mat4 translate;
uniform mat4 scale;
uniform mat4 rotate;
uniform mat4 view;
uniform mat4 projection;

out vec3 normal;

void main() {
	/*mat4 model = translate * rotate * scale;
	mat4 mvp   = projection * view * model;
	gl_Position = mvp * vec4(vPosition, 1.0f);
	normal = vec3(rotate * vec4(vNormal, 1.0f));*/

	mat4 normal = inverse(rotate * scale);
	mat4 model  = translate * rotate * scale;
	mat4 mvp    = projection * view * model;

	gl_Position = mvp * vec4(in_position, 1.0f);

	out_position = vec3(model  * vec4(in_position, 1.0f));
	out_normal   = normalize(in_normal * mat3(normal));
	out_uv       = in_uv;
}