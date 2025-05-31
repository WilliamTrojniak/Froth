#include "Camera.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/geometric.hpp"
#include "glm/trigonometric.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/rotate_vector.hpp"

namespace Froth {

Camera::Camera(const glm::vec3 &pos, float yaw, float pitch)
    : m_Pos(pos), m_Yaw(yaw), m_Pitch(pitch) {
}

glm::vec3 Camera::direction() const {
  // Assumes up is [0,0,1]
  glm::vec3 dir;
  dir.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
  dir.y = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
  dir.z = sin(glm::radians(m_Pitch));

  return glm::normalize(dir);
}

void Camera::rotate(float yaw, float pitch) {
  m_Yaw += yaw;
  m_Pitch += pitch;

  if (m_Pitch > 89.0f)
    m_Pitch = 89.0f;
  if (m_Pitch < -89.0f)
    m_Pitch = -89.0f;
}

void Camera::strafe(float speed) {
  glm::vec3 dir = direction();
  glm::vec3 right = glm::normalize(glm::cross(dir, m_Up));

  m_Pos += right * speed;
}

void Camera::moveForward(float speed) {
  glm::vec3 dir = direction();
  glm::vec3 right = glm::normalize(glm::cross(dir, m_Up));
  glm::vec3 forward = glm::normalize(glm::cross(m_Up, right));

  m_Pos += forward * speed;
}

void Camera::moveUp(float speed) {
  m_Pos += m_Up * speed;
}

glm::mat4 Camera::view() const {
  return glm::lookAt(m_Pos, m_Pos + direction(), m_Up);
}

} // namespace Froth
