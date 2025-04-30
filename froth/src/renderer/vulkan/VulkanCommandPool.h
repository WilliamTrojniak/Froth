#pragma once

#include "VulkanDevice.h"

namespace Froth {
class VulkanCommandPool {
public:
  VulkanCommandPool(const VulkanDevice &device, uint32_t queueFamilyIndex);
  ~VulkanCommandPool();

  VulkanCommandPool(VulkanCommandPool const &) = delete;
  VulkanCommandPool &operator=(VulkanCommandPool const &) = delete;

  operator VkCommandPool() const { return m_Pool; }

private:
  const VulkanDevice &m_Device;
  VkCommandPool m_Pool;

  void cleanup();
};

} // namespace Froth
