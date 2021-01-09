#version 440

in vec3 vs_texcoord;

out vec4 fs_color;

//Uniforms
uniform samplerCube skybox;

void main()
{
	vec4 spaceTex = texture(skybox, vs_texcoord);
	if (spaceTex.r > 0.50)
		discard;

	fs_color = spaceTex;
}