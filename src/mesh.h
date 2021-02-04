#ifndef _MESH_H
#define _MESH_H
#include <string>
#include <vector>

#include "glad/glad.h"
#include "glm/glm.hpp"
#include "shader.h"
#include "texture.h"

struct alignas(16) Vertex {
  alignas(16) glm::vec3 position;  // vertex position
  alignas(16) glm::vec3 normal;    // vertex normal
  alignas(8) glm::vec2 texcoords;  // texture coordinates
};

class Mesh {
 public:
  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;
  std::vector<Texture> textures;

  Mesh(const std::vector<Vertex>& vertices,
       const std::vector<unsigned int>& indices,
       const std::vector<Texture>& textures)
      : vertices(vertices), indices(indices), textures(textures) {
    setup();
  }

  void destroy() {
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteVertexArrays(1, &VAO);
    vertices.clear();
    indices.clear();

    for (auto& texture : textures) {
      texture.destroy();
    }
    textures.clear();
  }

  // draw mesh by given shader
  void draw(const Shader& shader) const {
    // setup textures
    std::size_t n_diffuse = 0;
    std::size_t n_specular = 0;
    for (std::size_t i = 0; i < textures.size(); ++i) {
      const Texture& texture = textures[i];
      const int textureUnitNumber = i;

      // set texture uniform
      switch (texture.textureType) {
        case TextureType::DIFFUSE: {
          const std::string uniformName =
              "diffuseTextures[" + std::to_string(n_diffuse) + "]";
          shader.setUniformTexture(uniformName, texture.id, textureUnitNumber);
          n_diffuse++;
          break;
        }
        case TextureType::SPECULAR: {
          const std::string uniformName =
              "specularTextures[" + std::to_string(n_specular) + "]";
          shader.setUniformTexture(uniformName, texture.id, textureUnitNumber);
          n_specular++;
          break;
        }
      }
    }

    // draw mesh
    glBindVertexArray(VAO);
    shader.activate();
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    shader.deactivate();
    glBindVertexArray(0);
  }

 private:
  GLuint VAO;
  GLuint VBO;
  GLuint EBO;

  // setup VBO, EBO, VAO
  void setup() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    // VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex),
                 vertices.data(), GL_STATIC_DRAW);

    // EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
                 indices.data(), GL_STATIC_DRAW);

    // position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          reinterpret_cast<void*>(0));
    // normal
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          reinterpret_cast<void*>(offsetof(Vertex, normal)));

    // texcoords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          reinterpret_cast<void*>(offsetof(Vertex, texcoords)));

    glBindVertexArray(0);
  }
};

#endif