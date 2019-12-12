#version 410

layout (location = 0) out vec4 gPosition;
layout (location = 1) out vec4 gNormal;
layout (location = 2) out vec4 gAlbedo;

layout (location = 0) in vec3 in_pos;
layout (location = 1) in vec3 in_normal;
layout (location = 2) in vec2 in_texCoords;

void main() {

    gPosition = vec4(in_pos, 1.0f); // store the fragment position vector
    gNormal = vec4(normalize(in_normal), 1.0f); // store the per-fragment normals
    gAlbedo = vec4(1.0f); // store the diffuse per-fragment color
}