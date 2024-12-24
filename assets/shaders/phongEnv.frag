
#version 410 core

// #define SPOT_LIGHT_NUM 4

out vec4 FragColor;

in vec2 uv;
in vec3 normal;
in vec3 worldPosition;

uniform sampler2D sampler;
uniform sampler2D specularMaskSampler;
uniform samplerCube envSampler;

// 光源参数
uniform vec3 ambientColor;
uniform float shiness;
uniform vec3 cameraPosition;

// 透明度
uniform float opacity;

struct DirectionalLight {
    vec3 direction;
    vec3 color;
    float specularIntensity;
};

struct PointLight {
    vec3 position;
    vec3 color;
    float specularIntensity;
    float k2;
    float k1;
    float kc;
};

struct SpotLight {
    vec3 position;
    vec3 targetDirection;
    vec3 color;
    float innerLine;
    float outerLine;
    float specularIntensity;
};

uniform DirectionalLight directionalLight;
// uniform PointLight pointLight;
// uniform SpotLight spotLights[SPOT_LIGHT_NUM];

vec3 calculateDiffuse(vec3 lightColor, vec3 objectColor, vec3 lightDir, vec3 normal) {
    float diffuse = clamp(dot(-lightDir, normal), 0.0, 1.0);
    vec3 diffuseColor = lightColor * diffuse * objectColor;

    return diffuseColor;
}

vec3 calculateSpecular(vec3 lightColor, vec3 lightDir, vec3 normal, vec3 viewDir, float intensity) {
    float dotResult = dot(-lightDir, normal);
    float flag = step(0.0, dotResult);
    vec3 lightReflect = normalize(reflect(lightDir, normal));
    float specular = clamp(dot(lightReflect, -viewDir), 0.0, 1.0);
    specular = pow(specular, shiness);
    // float specularMask = texture(specularMaskSampler, uv).r;
    vec3 specularColor = lightColor * specular * flag * intensity; // * specularMask; // 反射光直接反射, 并不是物体吸收后反射, 与物体颜色无关

    return specularColor;
}

vec3 calculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir) {
    vec3 objectColor = texture(sampler, uv).xyz;
    vec3 lightDir = normalize(light.direction);

    vec3 diffuseColor = calculateDiffuse(light.color, objectColor, lightDir, normal);
    vec3 specularColor = calculateSpecular(light.color, lightDir, normal, viewDir, light.specularIntensity);

    return diffuseColor + specularColor;
}

vec3 calculatePointLight(PointLight light, vec3 normal, vec3 viewDir) {
    vec3 objectColor = texture(sampler, uv).xyz;
    vec3 lightDir = normalize(worldPosition - light.position);

    float dist = length(worldPosition - light.position);
    float attenuation = 1.0 / (light.k2 * dist * dist + light.k1 * dist + light.kc);

    vec3 diffuseColor = calculateDiffuse(light.color, objectColor, lightDir, normal);
    vec3 specularColor = calculateSpecular(light.color, lightDir, normal, viewDir, light.specularIntensity);

    return (diffuseColor + specularColor) * attenuation;
}

vec3 calculateSpotLight(SpotLight light, vec3 normal, vec3 viewDir) {
    vec3 objectColor = texture(sampler, uv).xyz;
    vec3 lightDir = normalize(worldPosition - light.position);
    vec3 targetDir = normalize(light.targetDirection);

    float cGamma = dot(lightDir, targetDir);
    float intensity = clamp((cGamma - light.outerLine) / (light.innerLine - light.outerLine), 0.0, 1.0);

    vec3 diffuseColor = calculateDiffuse(light.color, objectColor, lightDir, normal);
    vec3 specularColor = calculateSpecular(light.color, lightDir, normal, viewDir, light.specularIntensity);

    return (diffuseColor + specularColor) * intensity;
}

vec3 calculateEnv(vec3 normal, vec3 viewDir) {
    vec3 reflectDir = normalize(reflect(viewDir, normal));
    vec3 color = texture(envSampler, reflectDir).xyz;
    return color;
}

void main() { 
    vec3 result = vec3(0.0);

    vec3 objectColor = texture(sampler, uv).xyz;
    float alpha = texture(sampler, uv).a;
    vec3 normalN = normalize(normal);
    vec3 viewDir = normalize(worldPosition - cameraPosition);

    result += calculateDirectionalLight(directionalLight, normalN, viewDir);
    // result += calculatePointLight(pointLight, normalN, viewDir);
    // for (int i = 0; i < SPOT_LIGHT_NUM; i++)
    //     result += calculateSpotLight(spotLights[i], normalN, viewDir);

    // 环境光
    vec3 ambientColor = objectColor * calculateEnv(normal, viewDir); // 直接照亮

    vec3 finalColor = result + ambientColor;
    
    FragColor = vec4(finalColor, alpha * opacity);
}