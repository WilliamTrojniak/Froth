#pragma once

#include "renderer/vulkan/VulkanCommandPool.h"
#include "renderer/vulkan/VulkanDevice.h"

namespace Froth {
class VulkanBuffer {
public:
  VulkanBuffer(const VulkanDevice &device, const VkDeviceSize &size, VkBufferUsageFlags usage, VkMemoryPropertyFlags memProperties);
  VulkanBuffer(VulkanBuffer &&) noexcept;
  ~VulkanBuffer();

  VulkanBuffer(VulkanBuffer const &) = delete;
  VulkanBuffer &operator=(VulkanBuffer const &) = delete;

  VkDeviceSize size() const { return m_Size; }
  operator VkBuffer() const { return m_Buffer; }

  void *map() const;
  void unmap() const;

  static void copyBuffer(const VulkanDevice &device, const VulkanBuffer &src, const VulkanBuffer &dest, const VulkanCommandPool &commandPool);

protected:
  const VulkanDevice &m_Device;

private:
  void cleanup();
  VkDeviceMemory m_Memory;
  VkBuffer m_Buffer;
  VkDeviceSize m_Size;
};

} // namespace Froth
