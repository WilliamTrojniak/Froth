#pragma once

#include "VulkanDevice.h"

namespace Froth {

class VulkanDescriptorSetLayout {
public:
  VulkanDescriptorSetLayout();
  VulkanDescriptorSetLayout(VulkanDescriptorSetLayout const &) = delete;
  void operator=(VulkanDescriptorSetLayout const &) = delete;
  ~VulkanDescriptorSetLayout();

  VkDescriptorSetLayout data() const { return m_DescriptorSetLayout; }

private:
  VkDescriptorSetLayout m_DescriptorSetLayout;

  void cleanup();
};

} // namespace Froth
