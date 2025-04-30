#pragma once

#include "VulkanCommandPool.h"

namespace Froth {
class VulkanBuffer {
public:
  VulkanBuffer(const VkDeviceSize &size, VkBufferUsageFlags usage, VkMemoryPropertyFlags memProperties);
  VulkanBuffer(VulkanBuffer &&) noexcept;
  ~VulkanBuffer();

  VulkanBuffer(VulkanBuffer const &) = delete;
  VulkanBuffer &operator=(VulkanBuffer const &) = delete;

  VkDeviceSize size() const { return m_Size; }
  operator VkBuffer() const { return m_Buffer; }

  void *map() const;
  void unmap() const;

  static void copyBuffer(const VulkanBuffer &src, const VulkanBuffer &dest, const VulkanCommandPool &commandPool);

private:
  void cleanup();
  VkDeviceMemory m_Memory;
  VkBuffer m_Buffer;
  VkDeviceSize m_Size;
};

} // namespace Froth
