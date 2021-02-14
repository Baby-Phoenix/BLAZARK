#version 440

layout (location = 0) in vec4 vert_position;
layout (location = 1) in vec3 vert_normal;
layout (location = 2) in vec2 vert_texcoord;

out vec4 vs_position;
out vec3 vs_normal;
out vec2 vs_texcoord;

uniform mat4 ViewProjection;
uniform mat4 ModelMatrix;
uniform mat3 NormalMatrix;

void main()
{
	vs_position = ModelMatrix * vert_position;
	vs_normal = NormalMatrix * vert_normal;
	vs_texcoord = vert_texcoord;

	gl_Position = ViewProjection * vs_position;
}