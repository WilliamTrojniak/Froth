#pragma once

#include "VulkanDevice.h"

namespace Froth {

class VulkanFence {
public:
  VulkanFence(bool signaled);
  VulkanFence(VulkanFence &&);
  ~VulkanFence();

  VulkanFence(VulkanFence const &) = delete;
  VulkanFence &operator=(VulkanFence const &) = delete;

  operator VkFence() const { return m_Fence; }

private:
  VkFence m_Fence;

  void cleanup();
};

} // namespace Froth
