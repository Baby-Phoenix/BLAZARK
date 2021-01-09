#version 440

layout (location = 0) in vec3 vertex_position;

out vec3 vs_texcoord;

uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

void main()
{
	vec4 pos = ProjectionMatrix * ViewMatrix  * vec4(vertex_position, 1.0f);
	gl_Position = pos.xyww;
	vs_texcoord = vertex_position;
}