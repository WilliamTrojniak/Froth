#pragma once

#include "VulkanDevice.h"

namespace Froth {

class VulkanPipelineLayout {
public:
  VulkanPipelineLayout(const std::vector<VkDescriptorSetLayout> &descSetLayouts);
  VulkanPipelineLayout(VulkanPipelineLayout const &) = delete;
  void operator=(VulkanPipelineLayout const &) = delete;
  ~VulkanPipelineLayout();

  operator VkPipelineLayout() const { return m_Layout; }

private:
  VkPipelineLayout m_Layout;

  void cleanup();
};

} // namespace Froth
