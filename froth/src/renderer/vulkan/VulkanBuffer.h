#pragma once

#include "VulkanCommandBuffer.h"

namespace Froth {
class VulkanBuffer {
public:
  VulkanBuffer() = default;
  VulkanBuffer(const VkDeviceSize &size, VkBufferUsageFlags usage, VkMemoryPropertyFlags memProperties);
  ~VulkanBuffer();

  VulkanBuffer(VulkanBuffer const &) = delete;
  VulkanBuffer &operator=(VulkanBuffer const &) = delete;
  VulkanBuffer(VulkanBuffer &&) noexcept;
  VulkanBuffer &operator=(VulkanBuffer &&) noexcept;

  VkDeviceSize size() const { return m_Size; }
  operator VkBuffer() const { return m_Buffer; }

  void *map() const;
  void unmap() const;

  static bool copyBuffer(const VulkanBuffer &src, const VulkanBuffer &dest, const VulkanCommandBuffer &commandBuffer);

  virtual void cleanup();

private:
  VkDeviceMemory m_Memory = nullptr;
  VkBuffer m_Buffer = nullptr;
  VkDeviceSize m_Size = 0;
};

} // namespace Froth
