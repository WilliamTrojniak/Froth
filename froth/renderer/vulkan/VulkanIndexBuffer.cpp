#include "VulkanIndexBuffer.h"
#include "renderer/vulkan/VulkanBuffer.h"
#include "renderer/vulkan/VulkanCommandPool.h"
#include "renderer/vulkan/VulkanDevice.h"
#include "renderer/vulkan/VulkanRenderer.h"
#include <algorithm>
#include <cstdint>

namespace Froth {

VulkanIndexBuffer::VulkanIndexBuffer(
    const VulkanDevice &device,
    const VulkanRenderer &renderer,
    const VkDeviceSize &size,
    const VulkanCommandPool &commandPool)
    : m_Renderer(renderer),
      m_CommandPool(commandPool),
      VulkanBuffer(device,
                   size,
                   VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
                   VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT),
      m_StagingBuffer(device,
                      size,
                      VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                      VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT) {
}

void VulkanIndexBuffer::write(size_t numIndices, const uint32_t *indexData) {
  size_t dataSize = numIndices * sizeof(uint32_t);
  dataSize = std::min(dataSize, static_cast<size_t>(size()));

  void *data = m_StagingBuffer.map();
  memcpy(data, indexData, dataSize);
  m_StagingBuffer.unmap();

  VulkanBuffer::copyBuffer(m_Device, m_StagingBuffer, *this, m_CommandPool);
  m_NumIndices = dataSize / sizeof(uint32_t);
}

void VulkanIndexBuffer::bind() {
  m_Renderer.bindIndexBuffer(*this);
}

} // namespace Froth
