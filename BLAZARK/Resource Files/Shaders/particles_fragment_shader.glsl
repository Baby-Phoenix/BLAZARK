#version 440

//in float lifetime;
in vec2 vs_texcoord;

out vec4 fragColor;

uniform sampler2D myTexture; 

in float lifetime;

void main()
{
   fragColor = vec4(1.0f, 1.0f* lifetime, 1.0f* lifetime, 0.7f);
   //fragColor = texture(myTexture, vs_texcoord);
}