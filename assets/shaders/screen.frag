
#version 410 core
out vec4 FragColor;

in vec2 uv;

uniform sampler2D screenTexSampler;

void main() { 
    FragColor = texture(screenTexSampler, uv);
}