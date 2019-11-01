#version 410 core

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

void main()
{
	mat4 model = translate * rotate * scale;
	mat4 mvp   = projection * view * model;

	gl_Position = mvp * vec4(in_position, 1.0F);

	out_position = (model * vec4(in_position, 1.0F)).xyz;
	out_normal   = (rotate * vec4(in_normal, 1.0F)).xyz;
	out_uv       = in_uv;
}
