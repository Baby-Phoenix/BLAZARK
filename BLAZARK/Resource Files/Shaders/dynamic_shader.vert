#version 440

layout (location = 0) in vec4 vert_position0;
layout (location = 1) in vec4 vert_position1;
layout (location = 2) in vec3 vert_normal0;
layout (location = 3) in vec3 vert_normal1;
layout (location = 4) in vec2 vert_texcoord;
layout (location = 5) in float specularShininess;
layout (location = 6) in vec3 ambientColour;
layout (location = 7) in vec3 diffuseColour;
layout (location = 8) in vec3 specularColour;
layout (location = 9) in float dissolve;

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
uniform float t;

void main()
{
	vs_position = ModelMatrix * mix(vert_position0, vert_position1, t);
	vs_normal = NormalMatrix * mix(vert_normal0, vert_normal1, t);
	vs_texcoord = vert_texcoord;
	specShininess = specularShininess;
	ambientCol = ambientColour;
	diffuseCol = diffuseColour;
	specularCol = specularColour;
	alpha = dissolve;

	gl_Position = ViewProjection * vs_position;
}