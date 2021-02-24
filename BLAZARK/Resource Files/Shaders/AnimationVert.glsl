#version 410

layout (location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout (location = 2) in vec2 InUV1;
layout (location = 3) in vec2 InUV2;

layout(location = 0) out vec2 Frame1;
layout(location = 1) out vec2 Frame2;

uniform mat4 ModelMatrix;
uniform mat4 ViewProjection;
uniform mat4 Projection;
uniform mat4 View;

void main()
{ 
	Frame1 = InUV1;
	Frame2 = InUV2;
 	gl_Position = ViewProjection * ModelMatrix * vec4(inPosition, 1.0);
   
}