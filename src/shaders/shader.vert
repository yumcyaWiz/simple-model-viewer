#version 330 core
layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vNormal;

out vec4 vColor;

void main() {
  gl_Position = vec4(vPos, 1.0);
  vColor = vec4(0.5 * (vNormal + 1.0), 1.0);
}