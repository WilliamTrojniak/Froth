#pragma once

#include "VulkanDevice.h"

namespace Froth {

class VulkanPipelineLayout {
public:
  VulkanPipelineLayout(const VulkanDevice &device, const std::vector<VkDescriptorSetLayout> &descSetLayouts);
  VulkanPipelineLayout(VulkanPipelineLayout const &) = delete;
  void operator=(VulkanPipelineLayout const &) = delete;
  ~VulkanPipelineLayout();

  VkPipelineLayout data() const { return m_Layout; }

private:
  const VulkanDevice &m_Device;
  VkPipelineLayout m_Layout;

  void cleanup();
};

} // namespace Froth
