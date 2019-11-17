#version 410

layout (location = 0) out vec4 gPosition;
layout (location = 1) out vec4 gNormal;
layout (location = 2) out vec4 gAlbedo;

//debugging
//out vec4 out_tex;
/*out vec4 gPosition;
out vec4 gNormal;
out vec4 gAlbedo;*/


/*/
//from the vert file
in vec3 in_pos;
in vec2 in_texCoords;
in vec3 in_normal;
/*/
layout (location = 0) in vec3 in_pos;
layout (location = 1) in vec3 in_normal;
layout (location = 2) in vec2 in_texCoords;
//*/

void main() {

	/*// store the fragment position vector in the first gbuffer texture
    gPosition = in_pos;
    // also store the per-fragment normals into the gbuffer
    gNormal = normalize(in_normal);
    // and the diffuse per-fragment color
    gAlbedo.rgb = vec3(0.95f);*/

    //out_tex = vec4(in_pos, 1.0f);
    //debugging:
    gPosition = vec4(in_pos, 1.0f);
    gNormal = vec4(normalize(in_normal), 1.0f);
    gAlbedo = vec4(0.95f);
}