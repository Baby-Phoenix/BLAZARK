#version 440

in vec4 vs_position;
in vec3 vs_normal;
in vec2 vs_texcoord;

out vec4 fs_colour;

uniform vec3 camPos;

uniform vec3 matColor = vec3(1.0f, 1.0f, 1.0f);
uniform sampler2D albedo;

uniform vec3 lightColor = vec3(0.9f, 0.9f, 0.9f);
uniform vec3 lightDir = normalize(vec3(-1.0f, -1.0f, -1.0f));
uniform vec3 ambientColor = vec3(1.0f, 1.0f, 1.0f);
uniform float ambientPower = 0.2f;

void main()
{
	vec3 normal = normalize(vs_normal); 

    vec3 eye = normalize(camPos - vs_position.xyz);
    vec3 toLight = -lightDir;

    vec3 avg = normalize(eye + toLight);

    float diffPower = max(dot(normal, toLight), 0.0f);
    vec3 diff = diffPower * lightColor;

    vec3 ambient = ambientPower * ambientColor;

    vec4 texCol = texture(albedo, vs_texcoord);
    vec3 result = (ambient + diff) * matColor * texCol.rgb;

    fs_colour = vec4(result, texCol.a);
}