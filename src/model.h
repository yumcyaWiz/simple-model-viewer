#ifndef _MODEL_H
#define _MODEL_H
#include <iostream>
#include <string>
#include <vector>
//
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <assimp/Importer.hpp>

#include "glad/glad.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
//
#include "mesh.h"

class Model {
 public:
  Model(const std::string& filepath) { loadModel(filepath); }

  // draw model by given shader
  void draw(const Shader& shader) const {
    for (std::size_t i = 0; i < meshes.size(); i++) {
      meshes[i].draw(shader);
    }
  }

 private:
  std::vector<Mesh> meshes;

  void loadModel(const std::string& filepath) {
    // load model with assimp
    Assimp::Importer importer;
    const aiScene* scene =
        importer.ReadFile(filepath, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
        !scene->mRootNode) {
      std::cerr << "[Assimp] " << importer.GetErrorString() << std::endl;
      return;
    }

    // process scene graph
    processNode(scene->mRootNode, scene);
  }

  void processNode(const aiNode* node, const aiScene* scene) {
    // process all the node's meshes
    for (std::size_t i = 0; i < node->mNumMeshes; ++i) {
      const aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
      meshes.push_back(processMesh(mesh, scene));
    }

    // recurse for each of its children
    for (std::size_t i = 0; i < node->mNumChildren; i++) {
      processNode(node->mChildren[i], scene);
    }
  }

  Mesh processMesh(const aiMesh* mesh, const aiScene* scene) {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    // vertices
    for (std::size_t i = 0; i < mesh->mNumVertices; ++i) {
      Vertex vertex;
      vertex.position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y,
                                  mesh->mVertices[i].z);
      vertex.normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y,
                                mesh->mNormals[i].z);
    }

    // indices
    for (std::size_t i = 0; i < mesh->mNumFaces; ++i) {
      const aiFace& face = mesh->mFaces[i];
      for (std::size_t j = 0; j < face.mNumIndices; ++j) {
        indices.push_back(face.mIndices[j]);
      }
    }

    return Mesh(vertices, indices);
  }
};

#endif