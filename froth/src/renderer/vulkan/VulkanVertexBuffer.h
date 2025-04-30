#pragma once

#include "src/renderer/VertexBuffer.h"
#include "src/renderer/vulkan/VulkanBuffer.h"
#include "src/renderer/vulkan/VulkanCommandPool.h"

namespace Froth {
class VulkanRenderer;

class VulkanVertexBuffer : public VulkanBuffer, public VertexBuffer {
public:
  VulkanVertexBuffer(const VulkanDevice &device,
                     const VulkanRenderer &renderer,
                     const VkDeviceSize &size,
                     const VulkanCommandPool &commandPool);

  virtual void write(size_t sizeBytes, const void *vertexData) override;
  virtual void bind() override;

  VulkanVertexBuffer(VulkanVertexBuffer const &) = delete;
  VulkanVertexBuffer &operator=(VulkanVertexBuffer const &) = delete;

private:
  const VulkanRenderer &m_Renderer;
  const VulkanCommandPool &m_CommandPool;
  const VulkanBuffer m_StagingBuffer;
};

} // namespace Froth
