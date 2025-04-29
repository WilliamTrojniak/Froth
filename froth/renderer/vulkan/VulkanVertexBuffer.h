#pragma once

#include "renderer/vulkan/VulkanBuffer.h"
#include "renderer/vulkan/VulkanCommandPool.h"
#include "renderer/vulkan/VulkanDevice.h"

namespace Froth {

class VulkanVertexBuffer : public VulkanBuffer {
public:
  VulkanVertexBuffer(const VulkanDevice &device, const VkDeviceSize &size, const VulkanCommandPool &commandPool);
  void write(const void *vertexData, size_t vertexDataSize);

  VulkanVertexBuffer(VulkanVertexBuffer const &) = delete;
  VulkanVertexBuffer &operator=(VulkanVertexBuffer const &) = delete;

private:
  const VulkanCommandPool &m_CommandPool;
  const VulkanBuffer m_StagingBuffer;
};

} // namespace Froth
