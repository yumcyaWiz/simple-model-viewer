#ifndef _MODEL_H
#define _MODEL_H
#include <filesystem>
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
  Model() {}
  Model(const std::string& filepath) { loadModel(filepath); }

  operator bool() const { return meshes.size() > 0; }

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
    const std::filesystem::path ps(filepath);
    processNode(scene->mRootNode, scene, ps.parent_path());

    // show info
    std::cout << "[Model] " << filepath << " loaded." << std::endl;
    std::cout << "[Model] number of meshes: " << meshes.size() << std::endl;

    std::size_t nVertices = 0;
    std::size_t nFaces = 0;
    std::size_t nTextures = 0;
    std::size_t nDiffuse = 0;
    std::size_t nSpecular = 0;
    for (std::size_t i = 0; i < meshes.size(); ++i) {
      nVertices += meshes[i].vertices.size();
      nFaces += meshes[i].indices.size() / 3;
      nTextures += meshes[i].textures.size();
    }
    std::cout << "[Model] number of vertices: " << nVertices << std::endl;
    std::cout << "[Model] number of faces: " << nFaces << std::endl;
    std::cout << "[Model] number of textures: " << nTextures << std::endl;
  }

  // draw model by given shader
  void draw(const Shader& shader) const {
    for (std::size_t i = 0; i < meshes.size(); i++) {
      meshes[i].draw(shader);
    }
  }

  void destroy() {
    for (std::size_t i = 0; i < meshes.size(); ++i) {
      meshes[i].destroy();
    }
    meshes.clear();
  }

 private:
  std::vector<Mesh> meshes;

  void processNode(const aiNode* node, const aiScene* scene,
                   const std::string& parentPath) {
    // process all the node's meshes
    for (std::size_t i = 0; i < node->mNumMeshes; ++i) {
      const aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
      meshes.push_back(processMesh(mesh, scene, parentPath));
    }

    for (std::size_t i = 0; i < node->mNumChildren; i++) {
      processNode(node->mChildren[i], scene, parentPath);
    }
  }

  Mesh processMesh(const aiMesh* mesh, const aiScene* scene,
                   const std::string& parentPath) {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    // vertices
    for (std::size_t i = 0; i < mesh->mNumVertices; ++i) {
      Vertex vertex;
      vertex.position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y,
                                  mesh->mVertices[i].z);

      if (mesh->mNormals) {
        vertex.normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y,
                                  mesh->mNormals[i].z);
      } else {
        vertex.normal = glm::vec3(0.0f, 0.0f, 0.0f);
      }

      if (mesh->mTextureCoords[0]) {
        vertex.texcoords = glm::vec2(mesh->mTextureCoords[0][i].x,
                                     mesh->mTextureCoords[0][i].y);
      } else {
        vertex.texcoords = glm::vec2(0.0f, 0.0f);
      }

      vertices.push_back(vertex);
    }

    // indices
    for (std::size_t i = 0; i < mesh->mNumFaces; ++i) {
      const aiFace& face = mesh->mFaces[i];
      for (std::size_t j = 0; j < face.mNumIndices; ++j) {
        indices.push_back(face.mIndices[j]);
      }
    }

    // materials
    if (scene->mMaterials[mesh->mMaterialIndex]) {
      aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

      const std::filesystem::path psParent(parentPath);

      // diffuse textures
      for (std::size_t i = 0;
           i < material->GetTextureCount(aiTextureType_DIFFUSE); ++i) {
        aiString str;
        material->GetTexture(aiTextureType_DIFFUSE, i, &str);
        const std::filesystem::path ps(str.C_Str());
        const std::string texturePath = (psParent / ps).native();

        textures.emplace_back(texturePath, TextureType::DIFFUSE);
      }

      // specular textures
      for (std::size_t i = 0;
           i < material->GetTextureCount(aiTextureType_SPECULAR); ++i) {
        aiString str;
        material->GetTexture(aiTextureType_SPECULAR, i, &str);
        const std::filesystem::path ps(str.C_Str());
        const std::string texturePath = (psParent / ps).native();

        textures.emplace_back(texturePath, TextureType::SPECULAR);
      }
    }

    return Mesh(vertices, indices, textures);
  }
};

#endif