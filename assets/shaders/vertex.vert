
#version 410 core
layout(location = 0) in vec3 aPos;  // 传入属性不允许修改
layout(location = 1) in vec2 aUV;
layout(location = 2) in vec3 aNormal;

out vec2 uv;
out vec3 normal;
out vec3 worldPosition;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat3 normalMatrix;

void main() {
  vec4 transformPosition = vec4(aPos, 1.0); // 转化为齐次坐标
  transformPosition = modelMatrix * transformPosition;
  worldPosition = transformPosition.xyz;  // 世界坐标系
  gl_Position = projectionMatrix * viewMatrix * transformPosition; // 最终转换到相机坐标系
  uv = aUV;
  // normal = transpose(inverse(mat3(modelMatrix))) * aNormal; // 法线需跟着变(CPU端算完再下放, 节约GPU资源)
  normal = normalMatrix * aNormal;
}