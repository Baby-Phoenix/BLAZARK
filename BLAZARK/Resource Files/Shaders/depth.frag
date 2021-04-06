#version 440

in vec4 FragPos;

uniform float Far_Plane;

void main() {
    float lightDistance = length(FragPos.xyz - vec3(0.0f, 0.0f, 0.0f));

    // Map to [0;1] range by dividing by far_plane
    lightDistance = lightDistance / Far_Plane;

    // Write this as modified depth
    gl_FragDepth = lightDistance;
}