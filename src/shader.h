#ifndef _SHADER_H
#define _SHADER_H

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "glad/glad.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

class Shader {
 private:
  const std::string vertexShaderFilepath;
  std::string vertexShaderSource;
  const std::string fragmentShaderSource;
  std::string fragmentShaderSource;
  GLuint vertexShader;
  GLuint fragmentShader;
  GLuint program;

  static std::string fileToString(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file) {
      std::cerr << "failed to open " << filepath << std::endl;
      return "";
    }

    std::stringstream ss;
    ss << file.rdbuf();
    file.close();

    return ss.str();
  }

  void compileShader() {
    // compile vertex shader
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
  }
};

#endif