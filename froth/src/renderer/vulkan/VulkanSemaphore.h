#pragma once

#include "VulkanDevice.h"

namespace Froth {

class VulkanSemaphore {
public:
  VulkanSemaphore();
  VulkanSemaphore(VulkanSemaphore &&);
  ~VulkanSemaphore();

  VulkanSemaphore(VulkanSemaphore const &) = delete;
  VulkanSemaphore &operator=(VulkanSemaphore const &) = delete;

  operator VkSemaphore() const { return m_Semaphore; }

private:
  VkSemaphore m_Semaphore;

  void cleanup();
};

} // namespace Froth
