#version 330 core
layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vNormal;

out vec3 position;
out vec3 normal;

layout(std140) uniform CameraBlock {
  mat4 view;
  mat4 projection;
};

void main() {
  gl_Position = projection * view * vec4(vPosition, 1.0);
  position = vPosition;
  normal = vNormal;
}