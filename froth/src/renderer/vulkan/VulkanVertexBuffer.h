#pragma once

#include "VulkanBuffer.h"
#include "VulkanCommandBuffer.h"

namespace Froth {
class VulkanRenderer;

class VulkanVertexBuffer : public VulkanBuffer {
public:
  VulkanVertexBuffer() = default;
  VulkanVertexBuffer(const VkDeviceSize &size);

  bool write(const VulkanCommandBuffer &commandBuffer, size_t sizeBytes, const void *vertexData);

  VulkanVertexBuffer(VulkanVertexBuffer const &) = delete;
  VulkanVertexBuffer &operator=(VulkanVertexBuffer const &) = delete;
  VulkanVertexBuffer(VulkanVertexBuffer &&) noexcept;
  VulkanVertexBuffer &operator=(VulkanVertexBuffer &&) noexcept;

  virtual void cleanup() override final;

private:
  VulkanBuffer m_StagingBuffer;
};

} // namespace Froth
