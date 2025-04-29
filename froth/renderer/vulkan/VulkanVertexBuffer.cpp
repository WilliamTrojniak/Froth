#include "VulkanVertexBuffer.h"
#include "renderer/vulkan/VulkanBuffer.h"
#include "renderer/vulkan/VulkanCommandPool.h"
#include "renderer/vulkan/VulkanDevice.h"
#include <algorithm>

namespace Froth {

VulkanVertexBuffer::VulkanVertexBuffer(const VulkanDevice &device, const VkDeviceSize &size, const VulkanCommandPool &commandPool)
    : m_CommandPool(commandPool),
      VulkanBuffer(device,
                   size,
                   VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
                   VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT),
      m_StagingBuffer(device,
                      size,
                      VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                      VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT) {
}

void VulkanVertexBuffer::write(const void *vertexData, size_t vertexDataSize) {
  vertexDataSize = std::min(vertexDataSize, static_cast<size_t>(size()));

  void *data = m_StagingBuffer.map();
  memcpy(data, vertexData, vertexDataSize);
  m_StagingBuffer.unmap();

  VulkanBuffer::copyBuffer(m_Device, m_StagingBuffer, *this, m_CommandPool);
}

} // namespace Froth
