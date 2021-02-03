#include <iostream>
//
#include "glad/glad.h"
//
#include "GLFW/glfw3.h"
#include "camera.h"
#include "model.h"

// globals
Camera camera;

void handleInput(GLFWwindow* window) {
  // close application
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
  }

  // camera movement
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    camera.move(CameraMovement::FORWARD, 0.01f);
  }
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    camera.move(CameraMovement::LEFT, 0.01f);
  }
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    camera.move(CameraMovement::BACKWARD, 0.01f);
  }
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    camera.move(CameraMovement::RIGHT, 0.01f);
  }
}

int main() {
  // initialize glfw
  if (!glfwInit()) {
    std::cerr << "failed to initialize GLFW" << std::endl;
    return EXIT_FAILURE;
  }

  // setup window and opengl context
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);  // required for Mac
  GLFWwindow* window =
      glfwCreateWindow(512, 512, "simple-model-viewer", nullptr, nullptr);
  if (!window) {
    std::cerr << "failed to create window" << std::endl;
    return EXIT_FAILURE;
  }
  glfwMakeContextCurrent(window);

  // initialize glad
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cerr << "failed to initialize glad" << std::endl;
    return EXIT_FAILURE;
  }

  glViewport(0, 0, 512, 512);

  // setup shader
  Shader shader("src/shaders/shader.vert", "src/shaders/shader.frag");

  // load model
  Model model(
      "assets/survival-guitar-backpack-low-poly/Survival_BackPack_2.fbx");

  // app loop
  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();

    // handle input
    handleInput(window);

    // set uniforms
    shader.setUniform("view", camera.computeViewMatrix());
    shader.setUniform("projection", camera.computeProjectionMatrix(512, 512));

    // rendering
    glClear(GL_COLOR_BUFFER_BIT);
    model.draw(shader);

    // swap buffer
    glfwSwapBuffers(window);
  }

  // exit
  model.destroy();
  shader.destroy();
  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}