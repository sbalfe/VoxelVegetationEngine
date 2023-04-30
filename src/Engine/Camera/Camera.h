//
// Created by shriller44 on 10/9/22.
//

#ifndef VOXEL_CAMERA_H
#define VOXEL_CAMERA_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <SDL2/SDL.h>
#include <fmt/format.h>
#include <iostream>


namespace {

  enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
  };

  // Default camera values
  const float YAW         = -90.0f;
  const float PITCH       =  0.0f;
  const float SPEED       =  0.01f;
  const float SENSITIVITY =  1.0f;
  const float ZOOM        =  45.0f;
}


class Camera
{
 public:
  explicit Camera(glm::vec3 position , glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);
  Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);
  [[nodiscard]] glm::mat4 GetViewMatrix() const;
  void Update(Camera_Movement direction, float deltaTime);
  void ProcessMouseMovement(double xoffset, double yoffset, GLboolean constrainPitch = true);
  void ProcessMouseScroll(float yoffset);
  void ProcessKeyboard(const Uint8* state, double delta_time);
  void ProcessMouse(double x_pos, double y_pos);

 public:
  glm::vec3 Position;
  glm::vec3 Front;
  glm::vec3 Up;
  glm::vec3 Right;
  glm::vec3 WorldUp;
  double Yaw;
  double Pitch;
  float MovementSpeed;
  float MouseSensitivity;
  double Zoom;
  double last_x_;
  double last_y_;
  bool first_mouse_;
 private:
  void updateCameraVectors();
};
#endif //VOXEL_CAMERA_H
