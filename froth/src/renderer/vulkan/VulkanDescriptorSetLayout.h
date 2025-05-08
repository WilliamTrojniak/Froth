#pragma once

#include "VulkanDevice.h"

namespace Froth {

class VulkanDescriptorSetLayout {
public:
  VulkanDescriptorSetLayout();
  VulkanDescriptorSetLayout(VulkanDescriptorSetLayout const &) = delete;
  void operator=(VulkanDescriptorSetLayout const &) = delete;

  VulkanDescriptorSetLayout(VulkanDescriptorSetLayout &&) noexcept;

  ~VulkanDescriptorSetLayout();

  VkDescriptorSetLayout data() const { return m_DescriptorSetLayout; }
  void cleanup();

private:
  VkDescriptorSetLayout m_DescriptorSetLayout;
};

} // namespace Froth
