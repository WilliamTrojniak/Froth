#pragma once

#include "glm/ext/matrix_float4x4.hpp"
namespace Froth {

class Camera {
public:
  Camera() = default;
  Camera(const glm::vec3 &pos, const glm::vec3 &rot);
  operator const glm::mat4 &() const { return m_View; }

  void translate(const glm::vec3 &trans);
  void rotate(float deg, const glm::vec3 &axis);

  void moveUp();
  void moveDown();
  void moveRight();
  void moveLeft();
  void moveBack();
  void moveForward();

  void lookUp();
  void lookDown();

private:
  glm::vec3 m_Pos;
  glm::vec3 m_Dir;
  glm::mat4 m_View;
};

} // namespace Froth
