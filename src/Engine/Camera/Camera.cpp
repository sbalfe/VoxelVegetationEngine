//
// Created by shriller44 on 10/9/22.
//

#include "Camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) : Front(glm::vec3(0.0f, 1.0f, 0.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
{
  Position = position;
  WorldUp = up;
  Yaw = yaw;
  Pitch = pitch;
  updateCameraVectors();
}

Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
{
  Position = glm::vec3(posX, posY, posZ);
  WorldUp = glm::vec3(upX, upY, upZ);
  Yaw = yaw;
  Pitch = pitch;
  last_x_ = static_cast<float>(1000) / 2.0f;
  last_y_ = static_cast<float>(600) / 2.0f;
  first_mouse_ = true;
  updateCameraVectors();
}

[[nodiscard]] glm::mat4 Camera::GetViewMatrix() const
{
  return glm::lookAt(Position, Position + Front, Up);
}

// processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
void Camera::Update(Camera_Movement direction, float deltaTime)
{
  float velocity = MovementSpeed * deltaTime;
  if (direction == FORWARD)
    Position += Front * velocity;
  if (direction == BACKWARD)
    Position -= Front * velocity;
  if (direction == LEFT)
    Position -= Right * velocity;
  if (direction == RIGHT)
    Position += Right * velocity;
}

// processes input received from a mouse input system. Expects the offset value in both the x and y direction.
void Camera::ProcessMouseMovement(double xoffset, double yoffset, GLboolean constrainPitch)
{

  xoffset *= MouseSensitivity;
  yoffset *= MouseSensitivity;

  Yaw   += xoffset;
  Pitch += yoffset;

  // make sure that when pitch is out of bounds, screen doesn't get flipped
  if (constrainPitch)
  {
    if (Pitch > 89.0f)
      Pitch = 89.0f;
    if (Pitch < -89.0f)
      Pitch = -89.0f;
  }

  // update Front, Right and Up Vectors using the updated Euler angles
  updateCameraVectors();
}

// processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
void Camera::ProcessMouseScroll(float yoffset)
{

  Zoom -= static_cast<float>(yoffset);

  if (Zoom < 1.0f)
    Zoom = 1.0f;
  if (Zoom > 45.0f)
    Zoom = 45.0f;
}

void  Camera::ProcessKeyboard(const Uint8* state, double delta_time){
  if (state[SDL_GetScancodeFromKey(SDLK_w)]){
    Update(FORWARD, delta_time);
  }
  else if (state[SDL_GetScancodeFromKey(SDLK_s)]){
    Update(BACKWARD, delta_time);
  }
  else if (state[SDL_GetScancodeFromKey(SDLK_a)]){
    Update(LEFT, delta_time);
  }
  else if (state[SDL_GetScancodeFromKey(SDLK_d)]){
    Update(RIGHT, delta_time);
  }
}

void Camera::ProcessMouse(double x_pos, double y_pos){

  if (first_mouse_)
  {
    last_x_ = x_pos;
    last_y_ = y_pos;
    first_mouse_ = false;
  }

  double x_offset = x_pos - last_x_;
  double y_offset = last_y_ - y_pos; // reversed since y-coordinates go from bottom to top

  last_x_ = x_pos;
  last_y_ = y_pos;

  ProcessMouseMovement(x_offset, y_offset);
}

void Camera::updateCameraVectors()
{
  // calculate the new Front vector
  glm::vec3 front;
  front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
  front.y = sin(glm::radians(Pitch));
  front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
  Front = glm::normalize(front);
  // also re-calculate the Right and Up vector
  Right = glm::normalize(glm::cross(Front, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
  Up    = glm::normalize(glm::cross(Right, Front));

}



