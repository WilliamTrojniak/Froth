#pragma once

#include "renderer/vulkan/VulkanBuffer.h"
#include "renderer/vulkan/VulkanCommandPool.h"
#include "renderer/vulkan/VulkanDevice.h"

namespace Froth {

class VulkanIndexBuffer : public VulkanBuffer {
public:
  VulkanIndexBuffer(const VulkanDevice &device, const VkDeviceSize &size, const VulkanCommandPool &commandPool);
  void write(const void *indexData, size_t indexDataSize);

  VulkanIndexBuffer(VulkanIndexBuffer const &) = delete;
  VulkanIndexBuffer &operator=(VulkanIndexBuffer const &) = delete;

private:
  const VulkanCommandPool &m_CommandPool;
  const VulkanBuffer m_StagingBuffer;
};

} // namespace Froth
