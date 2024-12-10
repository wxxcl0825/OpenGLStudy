
#version 410 core
layout(location = 0) in vec3 aPos;  // 传入属性不允许修改
layout(location = 1) in vec2 aUV;

out vec2 uv;

uniform mat4 transform;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main() {
  vec4 position = vec4(aPos, 1.0);
  position = projectionMatrix * viewMatrix * transform * position; // 最终转换到相机坐标系
  gl_Position = position;
  uv = aUV;
}