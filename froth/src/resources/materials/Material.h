#pragma once

#include "src/resources/shaders/Shader.h"
#include <memory>
namespace Froth {
class Material {
public:
  Material() {};
  Material(const std::shared_ptr<Shader> &vertexShader, const std::shared_ptr<Shader> &fragmentShader)
      : m_VertexShader(vertexShader), m_FragmentShader(fragmentShader) {}
  Material &operator=(Material &&o) {
    m_VertexShader = o.m_VertexShader;
    m_FragmentShader = o.m_FragmentShader;
    o.m_FragmentShader = nullptr;
    o.m_VertexShader = nullptr;
    return *this;
  };

  Shader &vert() const { return *m_VertexShader; }
  Shader &frag() const { return *m_FragmentShader; }

private:
  std::shared_ptr<Shader> m_VertexShader;
  std::shared_ptr<Shader> m_FragmentShader;
};

} // namespace Froth
