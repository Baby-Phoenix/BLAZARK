#version 440

layout (binding = 10) uniform sampler2D myTexture;
layout (binding = 11) uniform sampler2D myTexture2;

//in float lifetime;
in vec3 vs_texcoord;
in vec4 sColor;
in vec4 eColor;
in float lifetime;
in float texMixF;

out vec4 fragColor;


uniform float maxLifetime;

void main()
{
	vec4 tempCol;
	//if (lifetime < 1)
	// fragColor = vec4(1.0f * lifetime, 1.0f * lifetime, 1.0f * lifetime, f) * sColor * 0.2;
	//fragColor = pColor;
	

	//	frag_color.rgb = mix(texture1.rgb, texture2.rgb,t);
	float temp = texMixF;

	if(temp == 1)
	{
		//Mix tex
		vec4 texture1 = texture(myTexture, vs_texcoord.xy);
		vec4 texture2 = texture(myTexture2, vs_texcoord.xy);
		float t = lifetime / maxLifetime;

		fragColor = mix(texture2, texture1, t);

		//fragColor = texture(myTexture, vs_texcoord.xy);
		fragColor.a = fragColor.a * lifetime;
	}
	else
	{	
		//Single Tex
		fragColor = texture(myTexture, vs_texcoord.xy);
		fragColor.a = fragColor.a * lifetime;
	}
}