#version 440

in vec4 vs_position;
in vec3 vs_normal;
in vec2 vs_texcoord;
in float specShininess;
in vec3 ambientCol;
in vec3 diffuseCol;
in vec3 specularCol;
in float alpha;

out vec4 fs_colour;

uniform vec3 camPos;

uniform vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);

uniform float ambientPower = 0.75f;
uniform float diffusePower = 2.0f;

void main()
{
    vec3 normal = normalize(vs_normal);

    vec3 ambient = ambientPower * ambientCol;

    vec3 lightDir = normalize(vec3(0.0f, 0.0f, 0.0f) - vs_position.xyz);
    float diff = max(dot(lightDir, normal), 0.0f);
    vec3 diffuse = diffusePower * diff * lightColor;
    
    vec3 result = (ambient + diffuse) * diffuseCol;
    fs_colour = vec4(result, alpha);
}