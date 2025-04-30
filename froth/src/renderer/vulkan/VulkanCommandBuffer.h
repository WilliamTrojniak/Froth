#pragma once

#include "VulkanCommandPool.h"

namespace Froth {

class VulkanCommandBuffer {
public:
  VulkanCommandBuffer(const VulkanCommandPool &pool);
  ~VulkanCommandBuffer();

  VulkanCommandBuffer(VulkanCommandBuffer const &) = delete;
  VulkanCommandBuffer &operator=(VulkanCommandBuffer const &) = delete;

  VulkanCommandBuffer(VulkanCommandBuffer &&);
  operator VkCommandBuffer() const { return m_Buffer; }

private:
  const VulkanCommandPool &m_Pool;
  VkCommandBuffer m_Buffer;

  void cleanup();
};

} // namespace Froth
