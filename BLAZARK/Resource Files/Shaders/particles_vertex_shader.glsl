#version 440

layout ( location = 0 ) in vec3 vertex_position;
layout ( location = 4 ) in vec4 position;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

uniform float particleSize;
uniform vec4 sParticleColor;
uniform vec4 eParticleColor;

out vec4 sColor;
out vec4 eColor;
out float lifetime;
out vec3 vs_texcoord;


void main()
{
	vec4 position_viewspace = ViewMatrix * ModelMatrix* vec4( position.xyz , 1 );
    position_viewspace.xy += particleSize * (vertex_position.xy - vec2(0.5));

    vs_texcoord = vertex_position;

    gl_Position = ProjectionMatrix * position_viewspace;
 
    sColor = sParticleColor;
    eColor = eParticleColor;
    lifetime = position.w;
}