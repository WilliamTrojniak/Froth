#pragma once

#include "src/renderer/vulkan/VulkanShaderModule.h"
#include <memory>

namespace Froth {
class Material {
public:
  Material() {};
  Material(const std::shared_ptr<VulkanShaderModule> &vertexShader, const std::shared_ptr<VulkanShaderModule> &fragmentShader)
      : m_VertexShader(vertexShader), m_FragmentShader(fragmentShader) {}
  Material &operator=(Material &&o) {
    m_VertexShader = o.m_VertexShader;
    m_FragmentShader = o.m_FragmentShader;
    o.m_FragmentShader = nullptr;
    o.m_VertexShader = nullptr;
    return *this;
  };

  VulkanShaderModule &vert() const { return *m_VertexShader; }
  VulkanShaderModule &frag() const { return *m_FragmentShader; }

  void cleanup() {
    m_VertexShader = nullptr;
    m_FragmentShader = nullptr;
  }

private:
  std::shared_ptr<VulkanShaderModule> m_VertexShader;
  std::shared_ptr<VulkanShaderModule> m_FragmentShader;
};

} // namespace Froth
