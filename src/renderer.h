#ifndef _RENDERER_H
#define _RENDERER_H
#include "camera.h"
#include "model.h"
#include "shader.h"

enum class RenderMode { Position, Normal };

struct alignas(16) CameraBlock {
  alignas(64) glm::mat4 view;
  alignas(64) glm::mat4 projection;
};

class Renderer {
 public:
  Renderer(int width, int height)
      : width(width),
        height(height),
        renderMode(RenderMode::Normal),
        positionShader{"src/shaders/shader.vert", "src/shaders/position.frag"},
        normalShader{"src/shaders/shader.vert", "src/shaders/normal.frag"} {
    // set view and projection matrix
    cameraBlock.view = camera.computeViewMatrix();
    cameraBlock.projection = camera.computeProjectionMatrix(width, height);

    // setup camera UBO
    glGenBuffers(1, &cameraUBO);
    glBindBuffer(GL_UNIFORM_BUFFER, cameraUBO);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(CameraBlock), &cameraBlock,
                 GL_DYNAMIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, cameraUBO);
    positionShader.setUBO("CameraBlock", 0);
    normalShader.setUBO("CameraBlock", 0);
  }

  void render() {
    // render model
    switch (renderMode) {
      case RenderMode::Position:
        model.draw(positionShader);
        break;
      case RenderMode::Normal:
        model.draw(normalShader);
        break;
    }
  }

  void loadModel(const std::string& filepath) {
    // destroy previous model
    if (model) {
      model.destroy();
    }

    model.loadModel(filepath);
  }

  void setResolution(int width, int height) {
    this->width = width;
    this->height = height;

    // update projection matrix
    cameraBlock.projection = camera.computeProjectionMatrix(width, height);
    updateCameraUBO();
  }

  RenderMode getRenderMode() const { return renderMode; }
  void setRenderMode(const RenderMode& renderMode) {
    this->renderMode = renderMode;
  }

  float getCameraFOV() const { return camera.fov; }
  void setCameraFOV(float fov) {
    camera.fov = fov;

    // update projection matrix
    cameraBlock.projection = camera.computeProjectionMatrix(width, height);
    updateCameraUBO();
  }

  float getCameraMovementSpeed() const { return camera.movementSpeed; }
  void setCameraMovementSpeed(float movementSpeed) {
    camera.movementSpeed = movementSpeed;
  }

  void moveCamera(const CameraMovement& direction, float deltaTime) {
    camera.move(direction, deltaTime);

    // update view matrix
    cameraBlock.view = camera.computeViewMatrix();
    updateCameraUBO();
  }

  void lookAroundCamera(float dPhi, float dTheta) {
    camera.lookAround(dPhi, dTheta);

    // update view matrix
    cameraBlock.view = camera.computeViewMatrix();
    updateCameraUBO();
  }

  void destroy() {
    glDeleteBuffers(1, &cameraUBO);
    model.destroy();
    positionShader.destroy();
    normalShader.destroy();
  }

 private:
  int width;
  int height;
  RenderMode renderMode;
  Camera camera;
  Model model;

  Shader positionShader;
  Shader normalShader;

  GLuint cameraUBO;
  CameraBlock cameraBlock;

  void updateCameraUBO() {
    glBindBuffer(GL_UNIFORM_BUFFER, cameraUBO);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(CameraBlock), &cameraBlock,
                 GL_DYNAMIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
  }
};
#endif