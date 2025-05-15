#include "Camera.h"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/quaternion_geometric.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/geometric.hpp"
#include "glm/trigonometric.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/rotate_vector.hpp"

namespace Froth {

Camera::Camera(const glm::vec3 &pos, const glm::vec3 &dir)
    : m_Pos(pos), m_Dir(dir), m_View(glm::lookAt(pos, pos + dir, glm::vec3(0., 0., 1.))) {
}

void Camera::translate(const glm::vec3 &trans) {
  m_Pos += trans;

  m_View = glm::lookAt(m_Pos, m_Pos + m_Dir, glm::vec3(0.0f, 0.0f, 1.0f));
}

void Camera::rotate(float deg, const glm::vec3 &axis) {
  m_Dir = glm::rotate(m_Dir, deg * 0.1f, axis);

  m_View = glm::lookAt(m_Pos, m_Pos + m_Dir, glm::vec3(0.0f, 0.0f, 1.0f));
}

void Camera::lookUp() {
  glm::vec3 axis = glm::normalize(glm::cross(m_Dir, glm::vec3(0.0f, 0.0f, 1.0f)));
  m_Dir = glm::rotate(m_Dir, 0.1f, axis);

  m_View = glm::lookAt(m_Pos, m_Pos + m_Dir, glm::vec3(0.0f, 0.0f, 1.0f));
}
void Camera::lookDown() {
  glm::vec3 axis = glm::normalize(glm::cross(m_Dir, glm::vec3(0.0f, 0.0f, 1.0f)));
  m_Dir = glm::rotate(m_Dir, -0.1f, axis);

  m_View = glm::lookAt(m_Pos, m_Pos + m_Dir, glm::vec3(0.0f, 0.0f, 1.0f));
}

void Camera::moveUp() {
  glm::vec3 right = glm::cross(m_Dir, glm::vec3(0.0, 0.0, 1.0));
  m_Pos += 0.1f * glm::normalize(glm::cross(right, m_Dir));
  m_View = glm::lookAt(m_Pos, m_Pos + m_Dir, glm::vec3(0.0f, 0.0f, 1.0f));
}

void Camera::moveDown() {
  glm::vec3 right = glm::cross(m_Dir, glm::vec3(0.0, 0.0, 1.0));
  m_Pos -= 0.1f * glm::normalize(glm::cross(right, m_Dir));
  m_View = glm::lookAt(m_Pos, m_Pos + m_Dir, glm::vec3(0.0f, 0.0f, 1.0f));
}

void Camera::moveRight() {
  m_Pos += 0.1f * glm::cross(m_Dir, glm::vec3(0.f, 0.f, 1.f));
  m_View = glm::lookAt(m_Pos, m_Pos + m_Dir, glm::vec3(0.0f, 0.0f, 1.0f));
}

void Camera::moveLeft() {
  m_Pos -= 0.1f * glm::cross(m_Dir, glm::vec3(0.f, 0.f, 1.f));
  m_View = glm::lookAt(m_Pos, m_Pos + m_Dir, glm::vec3(0.0f, 0.0f, 1.0f));
}
void Camera::moveBack() {
  m_Pos -= 0.1f * m_Dir;

  m_View = glm::lookAt(m_Pos, m_Pos + m_Dir, glm::vec3(0.0f, 0.0f, 1.0f));
}
void Camera::moveForward() {
  m_Pos += 0.1f * m_Dir;
  m_View = glm::lookAt(m_Pos, m_Pos + m_Dir, glm::vec3(0.0f, 0.0f, 1.0f));
}

} // namespace Froth
