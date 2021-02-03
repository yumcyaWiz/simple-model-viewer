#ifndef _CAMERA_H
#define _CAMERA_H

#include "glad/glad.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

enum class CameraMovement {
  FORWARD,
  RIGHT,
  BACKWARD,
  LEFT,
};

class Camera {
 public:
  Camera()
      : camPos{0.0f},
        camForward{0.0, 0.0, -1.0f},
        camRight{1.0f, 0.0f, 0.0f},
        camUp{0.0f, 1.0f, 0.0f} {}

  glm::mat4 computeViewMatrix() const {
    return glm::lookAt(camPos, camPos + camForward, camUp);
  }

  // move camera
  void move(const CameraMovement& direction, float deltaTime) {
    float velocity = 10.0f * deltaTime;
    switch (direction) {
      case CameraMovement::FORWARD:
        camPos += velocity * camForward;
        break;
      case CameraMovement::RIGHT:
        camPos += velocity * camRight;
        break;
      case CameraMovement::BACKWARD:
        camPos -= velocity * camForward;
        break;
      case CameraMovement::LEFT:
        camPos -= velocity * camRight;
        break;
    }
  }

 private:
  glm::vec3 camPos;
  glm::vec3 camForward;
  glm::vec3 camRight;
  glm::vec3 camUp;
};

#endif