
#version 410 core
out vec4 FragColor;

in vec2 uv;
in vec3 normal;
in vec3 worldPosition;

uniform sampler2D sampler;
uniform sampler2D specularMaskSampler;

// 光源参数
// uniform vec3 lightDirection;
uniform vec3 lightPosition;
uniform vec3 lightColor;
uniform float specularIntensity;
uniform vec3 ambientColor;
uniform float shiness;

// 点光源控制
// uniform float k2;
// uniform float k1;
// uniform float kc;

// 聚光灯控制
uniform vec3 targetDirection;
uniform float innerLine;   // GPU计算cos消耗大, 在CPU计算后再下放(cos theta)
uniform float outerLine;   // cos beta

// 相机世界位置
uniform vec3 cameraPosition;

void main() { 
    // 光照通用数据
    vec3 objectColor = texture(sampler, uv).xyz;
    vec3 normalN = normalize(normal);
    vec3 lightDirN = normalize(worldPosition - lightPosition);
    vec3 viewDir = normalize(worldPosition - cameraPosition);   // 视线方向
    vec3 targetDirN = normalize(targetDirection);

    // 点光源衰减
    // float dist = length(worldPosition - lightPosition);
    // float attenuation = 1.0 / (k2 * dist * dist + k1 * dist + kc);

    // 聚光灯照射范围 clamp((cos gamma - cos beta) / (cos theta - cos beta))
    float cGamma = dot(lightDirN, targetDirN);
    float Spotintensity = clamp((cGamma - outerLine) / (innerLine - outerLine), 0.0, 1.0);

    // 漫反射
    float diffuse = clamp(dot(-lightDirN, normalN), 0.0, 1.0);   // 吸收率(cos theta), 处理 cos theta < 0 的情况(舍0)
    // 释放光强 = 输入光强 * cos theta * 物体颜色(反射率)
    vec3 diffuseColor = lightColor * diffuse * objectColor;

    // 高光反射
    // 防止背面光
    float dotResult = dot(-lightDirN, normalN);
    float flag = step(0.0, dotResult);  // bool(x > edge), flag = 1为正面照射(背面颜色为0)
    vec3 lightReflect = normalize(reflect(lightDirN, normalN));  // 反射方向
    float specular = clamp(dot(lightReflect, -viewDir), 0.0, 1.0);  // 反射率(cos theta)
    // 调整光斑大小(cos凸起区域过胖, 使用高次cos让它变瘦)
    specular = pow(specular, shiness);
    // 光强 * (cos theta)^N

    // 高光蒙版
    float specularMask = texture(specularMaskSampler, uv).r;

    // 高光反射强度控制 specularIntensity
    vec3 specularColor = lightColor * specular * flag * specularIntensity * specularMask; // 反射光直接反射, 并不是物体吸收后反射, 与物体颜色无关

    // 环境光
    vec3 ambientColor = objectColor * ambientColor; // 直接照亮

    vec3 finalColor = (diffuseColor + specularColor) * Spotintensity + ambientColor; // 漫反射叠加镜面反射
    
    FragColor = vec4(finalColor, 1.0);
}