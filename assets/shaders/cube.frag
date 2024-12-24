
#version 410 core
out vec4 FragColor;

in vec3 uvw;

uniform sampler2D sphericalSampler;

const float PI = 3.1415926535897932384626433832795;

vec2 uvwToUV(vec3 uvwN) {
    float phi = asin(uvwN.y);
    float theta = atan(uvwN.z, uvwN.x);
    float u = theta / (2.0 * PI) + 0.5;
    float v = phi / PI + 0.5;
    return vec2(u, v);
}

void main() { 
    FragColor = texture(sphericalSampler, uvwToUV(normalize(uvw)));
}