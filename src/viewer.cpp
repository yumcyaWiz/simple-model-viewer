#include <iostream>
//
#include "glad/glad.h"
//
#include "GLFW/glfw3.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
//
#include "camera.h"
#include "model.h"

// globals
unsigned int width = 512;
unsigned int height = 512;
Camera camera;

void handleInput(GLFWwindow* window, const ImGuiIO& io) {
  // close application
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
  }

  // camera movement
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    camera.move(CameraMovement::FORWARD, io.DeltaTime);
  }
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    camera.move(CameraMovement::LEFT, io.DeltaTime);
  }
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    camera.move(CameraMovement::BACKWARD, io.DeltaTime);
  }
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    camera.move(CameraMovement::RIGHT, io.DeltaTime);
  }
}

void framebufferSizeCallback(GLFWwindow* window, int _width, int _height) {
  width = _width;
  height = _height;
  glViewport(0, 0, width, height);
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
      glfwCreateWindow(width, height, "simple-model-viewer", nullptr, nullptr);
  if (!window) {
    std::cerr << "failed to create window" << std::endl;
    return EXIT_FAILURE;
  }
  glfwMakeContextCurrent(window);

  glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

  // initialize glad
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cerr << "failed to initialize glad" << std::endl;
    return EXIT_FAILURE;
  }

  // setup imgui
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  (void)io;

  // set imgui style
  ImGui::StyleColorsDark();

  // setup imgui backends
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init("#version 330 core");

  // enable depth test
  glEnable(GL_DEPTH_TEST);

  // setup shader
  Shader shader("src/shaders/shader.vert", "src/shaders/shader.frag");

  // load model
  Model model("assets/bunny/bunny.obj");

  // app loop
  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();

    // start imgui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("viewer");
    ImGui::End();

    // handle input
    handleInput(window, io);

    // set uniforms
    shader.setUniform("view", camera.computeViewMatrix());
    shader.setUniform("projection",
                      camera.computeProjectionMatrix(width, height));

    // render
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    model.draw(shader);

    // render imgui
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // swap buffer
    glfwSwapBuffers(window);
  }

  // exit
  model.destroy();
  shader.destroy();
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}