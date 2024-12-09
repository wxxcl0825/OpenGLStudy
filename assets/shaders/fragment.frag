
#version 410 core
out vec4 FragColor;
in vec3 color;
in vec2 uv;
uniform sampler2D grassSampler;
uniform sampler2D landSampler;
uniform sampler2D noiseSampler;
void main() { 
    vec4 grassColor = texture(grassSampler, uv);
    vec4 landColor = texture(landSampler, uv);
    vec4 noiseColor = texture(noiseSampler, uv);
    float weight = noiseColor.r;
    // vec4 finalColor = grassColor * (1.0 - weight) + landColor * weight;
    vec4 finalColor = mix(grassColor, landColor, weight); // weight是第二个参数的权重
    FragColor = vec4(finalColor.rgb, 1.0); 
}