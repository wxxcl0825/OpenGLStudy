
#version 410 core
out vec4 FragColor;

in vec2 uv;
in vec3 normal;

uniform sampler2D sampler;

// 光源参数
uniform vec3 lightDirection;
uniform vec3 lightColor;

void main() { 
    vec3 objectColor = texture(sampler, uv).xyz;
    // 漫反射相关数据
    vec3 normalN = normalize(normal);
    vec3 lightDirN = normalize(lightDirection);
    float diffuse = clamp(dot(-lightDirN, normalN), 0.0, 1.0);   // 吸收率(cos theta), 处理 cos theta < 0 的情况(舍0)
    // 释放光强 = 输入光强 * cos theta * 物体颜色(反射率)
    vec3 finalColor = lightColor * diffuse * objectColor;
    FragColor = vec4(finalColor, 1.0);
}