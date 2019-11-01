#version 410 core

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_normal;
layout (location = 2) in vec2 in_uv;

layout (location = 0) out vec4 out_colour;

void main()
{
	out_colour = vec4(0.2, 0.8, 0.4, 1.0);
}