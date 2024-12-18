
#version 410 core
layout(location = 0) in vec3 aPos;

out vec3 uvw;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat3 normalMatrix;

void main() {
  vec4 transformPosition = vec4(aPos, 1.0);
  transformPosition = modelMatrix * transformPosition;
  gl_Position = projectionMatrix * viewMatrix * transformPosition;
  gl_Position = gl_Position.xyww; // 深度始终为1, 永远通不过深度测试
  uvw = aPos;
}