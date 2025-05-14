#pragma once

#include "VulkanBuffer.h"
#include "VulkanCommandPool.h"

namespace Froth {
class VulkanRenderer;

class VulkanIndexBuffer : public VulkanBuffer {
public:
  VulkanIndexBuffer() = default;
  VulkanIndexBuffer(const VkDeviceSize &size);

  VulkanIndexBuffer(VulkanIndexBuffer const &) = delete;
  VulkanIndexBuffer &operator=(VulkanIndexBuffer const &) = delete;
  VulkanIndexBuffer(VulkanIndexBuffer &&) noexcept;
  VulkanIndexBuffer &operator=(VulkanIndexBuffer &&) noexcept;

  bool write(const VulkanCommandBuffer &commandBuffer, size_t numIndices, const uint32_t *indexData);

  size_t indexCount() const { return m_NumIndices; }

  virtual void cleanup() override final;

private:
  VulkanBuffer m_StagingBuffer;
  size_t m_NumIndices = 0;
};

} // namespace Froth
