#version 440

layout (binding = 10) uniform sampler2D myTexture;

//in float lifetime;
in vec3 vs_texcoord;
in vec4 sColor;
in vec4 eColor;

out vec4 fragColor;

//uniform sampler2D myTexture; 

in float lifetime;

void main()
{
	vec4 tempCol;
	//if (lifetime < 1)
	// fragColor = vec4(1.0f * lifetime, 1.0f * lifetime, 1.0f * lifetime, f) * sColor * 0.2;
	 //fragColor = pColor;
	
	fragColor = texture(myTexture, vs_texcoord.xy);
	fragColor.a = fragColor.a* lifetime;
}