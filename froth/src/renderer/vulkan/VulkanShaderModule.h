#pragma once

#include "vulkan/vulkan_core.h"
#include <vector>

namespace Froth {

class VulkanShaderModule {

public:
  VulkanShaderModule(const std::vector<char> &code, VkShaderStageFlagBits stage);
  VulkanShaderModule(VulkanShaderModule const &) = delete;
  void operator=(VulkanShaderModule const &) = delete;
  ~VulkanShaderModule();

  operator VkShaderModule() const { return m_ShaderModule; }

  VkPipelineShaderStageCreateInfo pipelineStageInfo() const;

private:
  VkShaderModule m_ShaderModule;
  VkShaderStageFlagBits m_Stage;

  void cleanup();
};

} // namespace Froth
