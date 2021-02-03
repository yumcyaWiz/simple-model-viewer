#ifndef _RENDERER_H
#define _RENDERER_H
#include "camera.h"
#include "model.h"
#include "shader.h"

class Renderer {
 public:
  Renderer()
      : positionShader{"src/shaders/shader.vert", "src/shaders/position.frag"},
        normalShader{"src/shaders/shader.vert", "src/shaders/normal.frag"} {}

  void render(int width, int height) const {
    // set uniforms
    normalShader.setUniform("view", camera.computeViewMatrix());
    normalShader.setUniform("projection",
                            camera.computeProjectionMatrix(width, height));

    // render model
    model.draw(normalShader);
  }

  void loadModel(const std::string& filepath) {
    // destroy previous model
    if (model) {
      model.destroy();
    }

    model.loadModel(filepath);
  }

  float getCameraFOV() const { return camera.fov; }
  void setCameraFOV(float fov) { camera.fov = fov; }

  float getCameraMovementSpeed() const { return camera.movementSpeed; }
  void setCameraMovementSpeed(float movementSpeed) {
    camera.movementSpeed = movementSpeed;
  }

  void moveCamera(const CameraMovement& direction, float deltaTime) {
    camera.move(direction, deltaTime);
  }

  void destroy() {
    model.destroy();
    positionShader.destroy();
    normalShader.destroy();
  }

 private:
  Camera camera;
  Model model;

  Shader positionShader;
  Shader normalShader;
};
#endif