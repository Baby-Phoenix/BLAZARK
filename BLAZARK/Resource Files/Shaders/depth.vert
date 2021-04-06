#version 440

layout (location = 0) in vec3 vPos;

uniform mat4 ModelMatrix;

void main() {
    gl_Position = ModelMatrix * vec4(vPos, 1.0);
}