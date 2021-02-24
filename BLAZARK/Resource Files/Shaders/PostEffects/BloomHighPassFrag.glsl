
#version 420

layout(binding = 0) uniform sampler2D uTex;

in vec2 TexCoords;

layout (location = 0) out vec4 fs_colour;
layout (location = 1) out vec4 fs_brightColour;

uniform vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);
uniform float threshold;

void main()
{
    vec4 texCol = texture(uTex, TexCoords);

    vec3 result = lightColor * texCol.rgb;
    fs_colour = vec4(result, texCol.a);

    float brightness = dot(fs_colour.rgb, vec3(0.2126, 0.7152, 0.0722));
    if (brightness > threshold)
        fs_brightColour = vec4(fs_colour.rgb, 1.0);
    else
        fs_brightColour = vec4(0.0, 0.0, 0.0, 1.0);
}