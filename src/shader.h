#ifndef _SHADER_H
#define _SHADER_H

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "glad/glad.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

class Shader {
 private:
  const std::string vertexShaderFilepath;
  std::string vertexShaderSource;
  const std::string fragmentShaderFilepath;
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
    vertexShaderSource = fileToString(vertexShaderFilepath);
    const char* vertexShaderSourceC = vertexShaderSource.c_str();
    glShaderSource(vertexShader, 1, &vertexShaderSourceC, nullptr);
    glCompileShader(vertexShader);

    // handle compilation error
    GLint success = 0;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (success == GL_FALSE) {
      std::cerr << "failed to compile vertex shader" << std::endl;

      GLint logSize = 0;
      glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &logSize);
      std::vector<GLchar> errorLog(logSize);
      glGetShaderInfoLog(vertexShader, logSize, &logSize, errorLog.data());
      std::string errorLogStr(errorLog.begin(), errorLog.end());
      std::cerr << errorLogStr << std::endl;

      glDeleteShader(vertexShader);
      return;
    }

    // compile fragment shader
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    fragmentShaderSource = fileToString(fragmentShaderFilepath);
    const char* fragmentShaderSourceC = fragmentShaderSource.c_str();
    glShaderSource(fragmentShader, 1, &fragmentShaderSourceC, nullptr);
    glCompileShader(fragmentShader);

    // handle compilation error
    success = 0;
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (success == GL_FALSE) {
      std::cerr << "failed to compile vertex shader" << std::endl;

      GLint logSize = 0;
      glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &logSize);
      std::vector<GLchar> errorLog(logSize);
      glGetShaderInfoLog(fragmentShader, logSize, &logSize, errorLog.data());
      std::string errorLogStr(errorLog.begin(), errorLog.end());
      std::cerr << errorLogStr << std::endl;

      glDeleteShader(fragmentShader);
      return;
    }
  }

  void linkShader() {
    // link shader program
    program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    glDetachShader(program, vertexShader);
    glDetachShader(program, fragmentShader);

    // handle link error
    int success = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (success == GL_FALSE) {
      std::cerr << "failed to link shaders" << std::endl;

      GLint logSize = 0;
      glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logSize);
      std::vector<GLchar> errorLog(logSize);
      glGetProgramInfoLog(program, logSize, &logSize, &errorLog[0]);
      std::string errorLogStr(errorLog.begin(), errorLog.end());
      std::cerr << errorLogStr << std::endl;

      glDeleteProgram(program);
      return;
    }
  }
};

#endif