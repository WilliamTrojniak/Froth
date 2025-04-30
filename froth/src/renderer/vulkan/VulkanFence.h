#pragma once

#include "VulkanDevice.h"

namespace Froth {

class VulkanFence {
public:
  VulkanFence(const VulkanDevice &device, bool signaled);
  VulkanFence(VulkanFence &&);
  ~VulkanFence();

  VulkanFence(VulkanFence const &) = delete;
  VulkanFence &operator=(VulkanFence const &) = delete;

  operator VkFence() const { return m_Fence; }

private:
  const VulkanDevice &m_Device;
  VkFence m_Fence;

  void cleanup();
};

} // namespace Froth
