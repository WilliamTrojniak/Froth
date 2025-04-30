#pragma once

#include "renderer/VertexBuffer.h"
#include "renderer/vulkan/VulkanBuffer.h"
#include "renderer/vulkan/VulkanCommandPool.h"

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
