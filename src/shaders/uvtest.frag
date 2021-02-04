#version 330 core
in vec3 position;
in vec3 normal;
in vec2 texCoords;

out vec4 fragColor;

uniform sampler2D testTexture;

void main() {
  fragColor = texture(testTexture, texCoords);
}