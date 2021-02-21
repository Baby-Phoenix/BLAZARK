#version 440

layout (location = 0) in vec4 vert_position;
layout (location = 1) in vec3 vert_normal;
layout (location = 2) in vec2 vert_texcoord;
layout (location = 3) in float specularShininess;
layout (location = 4) in vec3 ambientColour;
layout (location = 5) in vec3 diffuseColour;
layout (location = 6) in vec3 specularColour;
layout (location = 7) in float dissolve;

out vec4 vs_position;
out vec3 vs_normal;
out vec2 vs_texcoord;
out float specShininess;
out vec3 ambientCol;
out vec3 diffuseCol;
out vec3 specularCol;
out float alpha;

uniform mat4 ViewProjection;
uniform mat4 ModelMatrix;
uniform mat3 NormalMatrix;

void main()
{
	vs_position = ModelMatrix * vert_position;
	vs_normal = NormalMatrix * vert_normal;
	vs_texcoord = vert_texcoord;
	specShininess = specularShininess;
	ambientCol = ambientColour;
	diffuseCol = diffuseColour;
	specularCol = specularColour;
	alpha = dissolve;

	gl_Position = ViewProjection * vs_position;
}