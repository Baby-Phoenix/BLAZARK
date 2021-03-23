#version 440

layout ( location = 0 ) in vec3 vertex_position;
layout (location = 2) in vec2 vertex_texcoord;
layout ( location = 4 ) in vec4 position;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

uniform float particleSize;

out float lifetime;
out vec2 vs_texcoord;

void main()
{
   vec4 position_viewspace = ViewMatrix * ModelMatrix* vec4( position.xyz , 1 );
   position_viewspace.xy += particleSize * (vertex_position.xy - vec2(0.5));

   vs_texcoord = vec2(vertex_texcoord.x, vertex_texcoord.y * -1.f);

   gl_Position = ProjectionMatrix * position_viewspace;
 
   lifetime = position.w;
}