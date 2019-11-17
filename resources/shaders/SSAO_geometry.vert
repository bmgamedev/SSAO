#version 410

layout (location = 0) in vec3 in_pos;
layout (location = 1) in vec3 in_normal;
layout (location = 2) in vec2 in_texCoord;

layout (location = 0) out vec3 out_pos;
layout (location = 1) out vec3 out_normal;
layout (location = 2) out vec2 out_texCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {

	vec4 viewPos = view * model * vec4(in_pos, 1.0);
    out_pos = viewPos.xyz;
    out_texCoords = in_texCoord;

    mat3 normalMatrix = transpose(inverse(mat3(view * model)));
    //Normal = normalMatrix * (invertedNormals ? -in_normal : in_normal);
    out_normal = normalMatrix * in_normal;
    gl_Position = projection * viewPos;
}