
#version 410 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aUV;
layout(location = 2) in vec3 aNormal;

out vec2 uv;
out vec3 normal;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat3 normalMatrix;

void main() {
  vec4 transformPosition = vec4(aPos, 1.0);
  transformPosition = modelMatrix * transformPosition;
  gl_Position = projectionMatrix * viewMatrix * transformPosition;
  uv = aUV;
  normal = aNormal;
}