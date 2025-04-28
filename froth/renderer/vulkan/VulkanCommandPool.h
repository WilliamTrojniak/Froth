#pragma once

#include "renderer/vulkan/VulkanDevice.h"

namespace Froth {
class VulkanCommandPool {
public:
  VulkanCommandPool(const VulkanDevice &device, uint32_t queueFamilyIndex);
  ~VulkanCommandPool();

  VulkanCommandPool(VulkanCommandPool const &) = delete;
  VulkanCommandPool &operator=(VulkanCommandPool const &) = delete;

private:
  const VulkanDevice &m_Device;
  VkCommandPool m_Pool;

  void cleanup();
};

} // namespace Froth
