#version 330 core
in vec3 position;
in vec3 normal;

out vec4 fragColor;

void main() {
  fragColor = vec4(position, 1.0);
}