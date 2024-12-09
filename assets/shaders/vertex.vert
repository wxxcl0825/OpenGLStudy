
#version 410 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec2 aUV;

out vec3 color;
out vec2 uv;

uniform float time;

void main() {
  float scale = 1.0 / time;
  vec3 sPos = aPos * scale;
  gl_Position = vec4(sPos, 1.0);
  color = aColor;
  uv = aUV;
}