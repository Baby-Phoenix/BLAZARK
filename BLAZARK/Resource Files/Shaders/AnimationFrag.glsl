#version 420

layout(location = 0) in vec2 Frame1;
layout(location = 1) in vec2 Frame2;

out vec4 frag_color;


uniform sampler2D spritesheet;
uniform float u_Transparency = 1.0f;
uniform float t;

void main() 
{
	vec4 texture1 = texture(spritesheet, Frame1);
	vec4 texture2 = texture(spritesheet, Frame2);


	frag_color.rgb = mix(texture1.rgb, texture2.rgb,t);
	frag_color.a = mix(texture1.a, texture2.a,t) * u_Transparency;
}