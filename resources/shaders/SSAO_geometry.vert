#version 410

layout (location = 0) in vec3 in_pos;
layout (location = 1) in vec3 in_normal;
layout (location = 2) in vec2 in_texCoord;

/*/
out vec3 FragPos;
out vec2 TexCoords;
out vec3 Normal;
/*/
layout (location = 0) out vec3 out_pos;
layout (location = 1) out vec3 out_normal;
layout (location = 2) out vec2 out_texCoords;
//*/

//uniform bool invertedNormals;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform mat4 translate;
uniform mat4 scale;
uniform mat4 rotate;

void main() {
//
    //ORIGINAL:
	vec4 viewPos = view * model * vec4(in_pos, 1.0);
    out_pos = viewPos.xyz;
    out_texCoords = in_texCoord;

    mat3 normalMatrix = transpose(inverse(mat3(view * model)));
    //Normal = normalMatrix * (invertedNormals ? -in_normal : in_normal);
    out_normal = normalMatrix * in_normal;
    gl_Position = projection * viewPos;
/*/
    //AMENDED: (mine)
    mat4 normal = inverse(rotate * scale);
	mat4 model  = translate * rotate * scale;
	mat4 mvp    = projection * view * model;

	gl_Position = mvp * vec4(in_pos, 1.0f);

	out_pos = vec3(model  * vec4(in_pos, 1.0f));
	out_normal   = normalize(in_normal * mat3(normal));
	out_texCoords = in_texCoord;
//*/
}