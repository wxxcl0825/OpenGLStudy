
#version 410 core
out vec4 FragColor;

in vec3 color;
in vec2 uv;

uniform sampler2D sampler;
uniform float width;
uniform float height;

void main() { 
    // FragColor = texture(sampler, uv); 
    // 获取纹素具体位置
    vec2 location = uv * vec2(width, height);
    // 获取xy方向变化量
    vec2 dx = dFdx(location);
    vec2 dy = dFdy(location);
    // 选择最大的delta
    float maxDelta = sqrt(max(dot(dx, dx), dot(dy, dy)));   // 对角线长度
    float L = log2(maxDelta);
    // 上取整
    int level = max(int(L + 0.5), 0);

    FragColor = textureLod(sampler, uv, level);
}