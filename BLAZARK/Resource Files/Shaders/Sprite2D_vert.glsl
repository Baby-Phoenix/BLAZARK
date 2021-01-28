#version 440
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 normal;

out vec2 TexCoords;
out vec3 normal0;
out vec3 fragPosition;

uniform mat4 ModelMatrix;
uniform mat4 ViewProjection;
uniform mat4 Projection;

void main()
{
        mat4 View = ViewProjection / Projection;
    gl_Position = ViewProjection * ModelMatrix * vec4(position, 1.0);
    TexCoords.x = texCoord.x;
    TexCoords.y = 1.0 - texCoord.y;   
    normal0 = mat3(View) * mat3(ModelMatrix) * normal;
    fragPosition = (ViewProjection * ModelMatrix * vec4(position, 1.0)).rgb;
}