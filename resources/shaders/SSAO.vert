#version 410

layout (location = 0) in vec3 in_pos;
layout (location = 2) in vec2 in_texCoords;

out vec2 TexCoords;

void main() {

	TexCoords = in_texCoords;
    gl_Position = vec4(in_pos, 1.0f);
}