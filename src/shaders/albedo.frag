#version 330 core
in vec3 position;
in vec3 normal;
in vec2 texCoords;

out vec4 fragColor;

uniform sampler2D diffuseTextures[100];
uniform sampler2D specularTextures[100];

void main() {
  fragColor = texture(diffuseTextures[0], texCoords);
}