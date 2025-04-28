#pragma once

#include "renderer/vulkan/VulkanDevice.h"

namespace Froth {

class VulkanSemaphore {
public:
  VulkanSemaphore(const VulkanDevice &device);
  VulkanSemaphore(VulkanSemaphore &&);
  ~VulkanSemaphore();

  VulkanSemaphore(VulkanSemaphore const &) = delete;
  VulkanSemaphore &operator=(VulkanSemaphore const &) = delete;

  operator VkSemaphore() const { return m_Semaphore; }

private:
  const VulkanDevice &m_Device;
  VkSemaphore m_Semaphore;

  void cleanup();
};

} // namespace Froth
