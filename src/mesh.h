#ifndef _MESH_H
#define _MESH_H
#include <string>
#include <vector>

#include "glad/glad.h"
#include "glm/glm.hpp"
#include "shader.h"

struct Vertex {
  glm::vec3 position;  // vertex position
  glm::vec3 normal;    // vertex normal
};

class Mesh {
 public:
  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;

  Mesh(const std::vector<Vertex>& vertices,
       const std::vector<unsigned int>& indices)
      : vertices(vertices), indices(indices) {
    setup();
  }

  void destroy() const {
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteVertexArrays(1, &VAO);
  }

  // draw mesh by given shader
  void draw(const Shader& shader) const {
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

    glBindVertexArray(0);
  }
};

#endif