#version 440
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;


out vec2 TexCoords;

uniform mat4 ModelMatrix;
uniform mat4 ViewProjection;
uniform mat4 Projection;
uniform mat4 View;

void main()
{
    gl_Position = ViewProjection * ModelMatrix * vec4(position, 1.0);
    TexCoords.x = texCoord.x;   
    TexCoords.y = texCoord.y;
}