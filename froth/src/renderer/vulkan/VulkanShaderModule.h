#pragma once

#include "VulkanDevice.h"
#include <vector>

namespace Froth {

class VulkanShaderModule {

public:
  VulkanShaderModule(const VulkanDevice &device, const std::vector<char> &code);
  VulkanShaderModule(VulkanShaderModule const &) = delete;
  void operator=(VulkanShaderModule const &) = delete;
  ~VulkanShaderModule();

  operator VkShaderModule() const { return m_ShaderModule; }

private:
  const VulkanDevice &m_Device;
  VkShaderModule m_ShaderModule;

  void cleanup();
};

} // namespace Froth
