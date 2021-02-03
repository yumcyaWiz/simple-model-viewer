#version 330 core
layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vNormal;

out vec3 position;
out vec3 normal;

uniform mat4 view;
uniform mat4 projection;

void main() {
  gl_Position = projection * view * vec4(vPosition, 1.0);
  position = vPosition;
  normal = vNormal;
}