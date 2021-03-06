#include <iostream>
#include <memory>
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
#include "renderer.h"

// globals
int width = 1600;
int height = 900;
std::unique_ptr<Renderer> renderer;

void handleInput(GLFWwindow* window, const ImGuiIO& io) {
  // close application
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
  }

  // camera movement
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    renderer->moveCamera(CameraMovement::FORWARD, io.DeltaTime);
  }
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    renderer->moveCamera(CameraMovement::LEFT, io.DeltaTime);
  }
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    renderer->moveCamera(CameraMovement::BACKWARD, io.DeltaTime);
  }
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    renderer->moveCamera(CameraMovement::RIGHT, io.DeltaTime);
  }

  // camera look around
  if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
    const float orbitSpeed = 1.0f;
    renderer->lookAroundCamera(orbitSpeed * io.MouseDelta.x,
                               orbitSpeed * io.MouseDelta.y);
  }
}

void framebufferSizeCallback([[maybe_unused]] GLFWwindow* window, int _width,
                             int _height) {
  width = _width;
  height = _height;
  glViewport(0, 0, width, height);
  renderer->setResolution(width, height);
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
  glfwWindowHint(GLFW_SAMPLES, 4);                      // 4x MSAA
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
  // enable MSAA
  glEnable(GL_MULTISAMPLE);

  // setup renderer
  renderer = std::make_unique<Renderer>(width, height);

  // app loop
  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();

    // start imgui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // imgui
    ImGui::Begin("viewer");

    static char modelFilepath[100] = {"assets/sponza/sponza.obj"};
    ImGui::InputText("Model", modelFilepath, 100);
    if (ImGui::Button("Load Model")) {
      renderer->loadModel(modelFilepath);
    }

    static RenderMode renderMode = renderer->getRenderMode();
    if (ImGui::Combo("Render Mode", reinterpret_cast<int*>(&renderMode),
                     "Position\0Normal\0TexCoords\0Diffuse\0Specular\0\0")) {
      renderer->setRenderMode(renderMode);
    }

    static float fov = renderer->getCameraFOV();
    if (ImGui::InputFloat("FOV", &fov)) {
      renderer->setCameraFOV(fov);
    }

    static float movementSpeed = renderer->getCameraMovementSpeed();
    if (ImGui::InputFloat("Movement Speed", &movementSpeed)) {
      renderer->setCameraMovementSpeed(movementSpeed);
    }

    static float lookAroundSpeed = renderer->getCameraLookAroundSpeed();
    if (ImGui::InputFloat("Look Around Speed", &lookAroundSpeed)) {
      renderer->setCameraLookAroundSpeed(lookAroundSpeed);
    }

    if (ImGui::Button("Reset Camera")) {
      renderer->resetCamera();
    }

    ImGui::End();

    // handle input
    handleInput(window, io);

    // render
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    renderer->render();

    // render imgui
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // swap buffer
    glfwSwapBuffers(window);
  }

  // exit
  renderer->destroy();
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}