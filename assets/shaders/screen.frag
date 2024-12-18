
#version 410 core
out vec4 FragColor;

in vec2 uv;

uniform sampler2D screenTexSampler;

uniform float texWidth;
uniform float texHeight;

vec3 colorInvert(vec3 color) {
    return vec3(1.0) - color;
}

vec3 gray(vec3 color) {
    float gray = dot(color, vec3(0.2126, 0.7152, 0.0722));
    return vec3(gray);
}

vec3 blur() {
    float du = 1.0 / texWidth;
    float dv = 1.0 / texHeight;

    vec2 offset[9] = vec2[](
        vec2(-du, -dv), vec2(0.0, -dv), vec2(du, -dv),
        vec2(-du, 0.0), vec2(0.0, 0.0), vec2(du, 0.0),
        vec2(-du, dv), vec2(0.0, dv), vec2(du, dv)
    );

    float kernel[9] = float[](
        1.0, 2.0, 1.0,
        2.0, 4.0, 2.0,
        1.0, 2.0, 1.0
    );

    vec3 sumColor = vec3(0.0);

    for (int i = 0; i < 9; i++) {
        sumColor += texture(screenTexSampler, uv + offset[i]).rgb * kernel[i] / 16.0;
    }

    return sumColor;
}

void main() { 
    FragColor = vec4(texture(screenTexSampler, uv).rgb, 1.0);
}