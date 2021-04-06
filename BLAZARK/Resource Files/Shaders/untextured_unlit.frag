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

layout (binding = 15) uniform samplerCube depthMap;

uniform vec3 camPos;

uniform vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);

uniform float Far_Plane;

uniform float ambientPower = 0.75f;
uniform float diffusePower = 2.0f;
uniform float specularPower = 0.5;

uniform bool Shadows;

vec3 lightPos = vec3(0.0f, 0.0f, 0.0f);

vec3 gridSamplingDisk[20] = vec3[] (
    vec3(1, 1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1, 1,  1),
    vec3(1, 1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1),
    vec3(1, 1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1, 1,  0),
    vec3(1, 0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1, 0, -1),
    vec3(0, 1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0, 1, -1)
);

float ShadowCalculation(vec3 fragPos) {
    vec3 fragToLight = fragPos - lightPos;

    float currentDepth = length(fragToLight);

    float shadow = 0.0;
    int samples = 20;
    float bias = 0.15;
    float viewDistance = length(camPos - fragPos);
    float diskRadius = (1.0 + (viewDistance / Far_Plane)) / 25.0;

    for (int i = 0; i < samples; ++i) {
        float closestDepth = texture(depthMap, fragToLight + gridSamplingDisk[i] * diskRadius).r;
        closestDepth *= Far_Plane; // Convert [0;1] mapping to [0;far_plane] mapping

        if (currentDepth - bias > closestDepth)
            shadow += 1.0;
    }

    shadow /= float(samples);

    return shadow;
}

void main() {
    vec3 normal = normalize(vs_normal);

    // Ambient
    vec3 ambient = ambientPower * ambientCol;

    // Diffuse
    vec3 lightDir = normalize(lightPos - vs_position.xyz);
    float diff = max(dot(lightDir, normal), 0.0f);
    vec3 diffuse = diffusePower * diff * lightColor;

    // Specular
    vec3 viewDir = normalize(camPos - vs_position.xyz);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularPower * spec * specularCol;

    // Shadows
    float shadow = Shadows ? ShadowCalculation(vs_position.xyz) : 0.0;

    vec3 result = (ambient + diffuse + specular) * diffuseCol;
    //vec3 result = (ambient + (1.0 - shadow) * (diffuse + specular)) * diffuseCol;
    fs_colour = vec4(result, alpha);
}