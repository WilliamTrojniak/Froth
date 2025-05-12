#include "VulkanVertexBuffer.h"
#include "VulkanRenderer.h"
#include <algorithm>
#include <string.h>

namespace Froth {

VulkanVertexBuffer::VulkanVertexBuffer(const VulkanRenderer &renderer,
                                       const VkDeviceSize &size,
                                       const VulkanCommandPool &commandPool)
    : m_Renderer(renderer),
      m_CommandPool(commandPool),
      VulkanBuffer(size,
                   VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
                   VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT),
      m_StagingBuffer(size,
                      VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                      VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT) {
}

void VulkanVertexBuffer::write(size_t vertexDataSize, const void *vertexData) {
  vertexDataSize = std::min(vertexDataSize, static_cast<size_t>(size()));

  void *data = m_StagingBuffer.map();
  memcpy(data, vertexData, vertexDataSize);
  m_StagingBuffer.unmap();

  VulkanBuffer::copyBuffer(m_StagingBuffer, *this, m_CommandPool);
}

void VulkanVertexBuffer::bind() {
  m_Renderer.bindVertexBuffer(*this);
}

} // namespace Froth
