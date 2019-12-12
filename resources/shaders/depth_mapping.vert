#version 410

layout (location = 0) in vec3 in_pos;

uniform mat4 lightSpaceMatrix;
uniform mat4 model;

void main(){

    gl_Position = lightSpaceMatrix * model * vec4(in_pos, 1.0);

}