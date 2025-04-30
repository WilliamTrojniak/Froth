#pragma once

#include "VulkanBuffer.h"
#include "VulkanCommandPool.h"
#include "src/renderer/IndexBuffer.h"

namespace Froth {
class VulkanRenderer;

class VulkanIndexBuffer : public VulkanBuffer, public IndexBuffer {
public:
  VulkanIndexBuffer(const VulkanRenderer &renderer,
                    const VkDeviceSize &size,
                    const VulkanCommandPool &commandPool);

  VulkanIndexBuffer(VulkanIndexBuffer const &) = delete;
  VulkanIndexBuffer &operator=(VulkanIndexBuffer const &) = delete;

  virtual void bind() override;
  virtual void write(size_t numIndices, const uint32_t *indexData) override;

  size_t indexCount() const { return m_NumIndices; }

private:
  const VulkanRenderer &m_Renderer;
  const VulkanCommandPool &m_CommandPool;
  const VulkanBuffer m_StagingBuffer;
  size_t m_NumIndices = 0;
};

} // namespace Froth
