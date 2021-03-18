#version 440

in vec4 vs_position;
in vec3 vs_normal;
in vec2 vs_texcoord;

layout (location = 0) out vec4 fs_colour;
layout (location = 1) out vec4 fs_brightColour;

uniform sampler2D albedo;

uniform vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);

void main()
{
    vec4 texCol = texture(albedo, vs_texcoord);

    vec3 result = lightColor * texCol.rgb;
    fs_colour = vec4(result, texCol.a);

    float brightness = dot(fs_colour.rgb, vec3(0.2126, 0.7152, 0.0722));
    if (brightness > 1.0)
        fs_brightColour = vec4(fs_colour.rgb, 1.0);
    else
        fs_brightColour = vec4(0.0, 0.0, 0.0, 1.0);
}