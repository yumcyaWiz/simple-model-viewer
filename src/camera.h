#ifndef _CAMERA_H
#define _CAMERA_H

#include "glad/glad.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

enum class CameraMovement {
  FORWARD,
  BACKWARD,
  RIGHT,
  LEFT,
  UP,
  DOWN,
};

class Camera {
 public:
  Camera()
      : camPos{0.0f},
        camForward{0.0, 0.0, -1.0f},
        camRight{1.0f, 0.0f, 0.0f},
        camUp{0.0f, 1.0f, 0.0f},
        movementSpeed(1.0f) {}

  float getMovementSpeed() const { return movementSpeed; }
  void setMovementSpeed(float speed) { movementSpeed = speed; }

  glm::mat4 computeViewMatrix() const {
    return glm::lookAt(camPos, camPos + camForward, camUp);
  }

  glm::mat4 computeProjectionMatrix(int width, int height) const {
    return glm::perspective(glm::radians(45.0f),
                            static_cast<float>(width) / height, 0.1f, 100.0f);
  }

  // move camera
  void move(const CameraMovement& direction, float deltaTime) {
    const float velocity = movementSpeed * deltaTime;
    switch (direction) {
      case CameraMovement::FORWARD:
        camPos += velocity * camForward;
        break;
      case CameraMovement::BACKWARD:
        camPos -= velocity * camForward;
        break;
      case CameraMovement::RIGHT:
        camPos += velocity * camRight;
        break;
      case CameraMovement::LEFT:
        camPos -= velocity * camRight;
        break;
      case CameraMovement::UP:
        camPos += velocity * camUp;
        break;
      case CameraMovement::DOWN:
        camPos -= velocity * camUp;
        break;
    }
  }

 private:
  glm::vec3 camPos;
  glm::vec3 camForward;
  glm::vec3 camRight;
  glm::vec3 camUp;

  float movementSpeed;
};

#endif