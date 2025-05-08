#pragma once

#include "VulkanCommandBuffer.h"

namespace Froth {
class VulkanCommandPool {

public:
  VulkanCommandPool() = default;
  VulkanCommandPool(uint32_t queueFamilyIndex);
  ~VulkanCommandPool();

  VulkanCommandPool(VulkanCommandPool const &) = delete;
  VulkanCommandPool &operator=(VulkanCommandPool const &) = delete;

  VulkanCommandPool(VulkanCommandPool &&) noexcept;
  VulkanCommandPool &operator=(VulkanCommandPool &&) noexcept;

  operator VkCommandPool() const { return m_Pool; }

  VulkanCommandBuffer AllocateCommandBuffer() const;

private:
  VkCommandPool m_Pool = nullptr;

  void cleanup();
};

} // namespace Froth
