#pragma once

#include "VulkanDevice.h"
#include <vector>

namespace Froth {

class VulkanDescriptorSetLayout {
public:
  VulkanDescriptorSetLayout() = default;
  VulkanDescriptorSetLayout(const std::vector<VkDescriptorSetLayoutCreateInfo> &data);
  VulkanDescriptorSetLayout(VulkanDescriptorSetLayout const &) = delete;
  void operator=(VulkanDescriptorSetLayout const &) = delete;

  VulkanDescriptorSetLayout(VulkanDescriptorSetLayout &&) noexcept;
  VulkanDescriptorSetLayout &operator=(VulkanDescriptorSetLayout &&) noexcept;

  ~VulkanDescriptorSetLayout();

  VkDescriptorSetLayout data() const { return m_DescriptorSetLayout; }
  void cleanup();

private:
  VkDescriptorSetLayout m_DescriptorSetLayout;
};

} // namespace Froth
