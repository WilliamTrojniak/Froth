#include "VulkanIndexBuffer.h"
#include "VulkanBuffer.h"
#include "VulkanCommandBuffer.h"
#include "VulkanRenderer.h"
#include "src/core/logger/Logger.h"
#include <algorithm>
#include <cstdint>
#include <string.h>

namespace Froth {

VulkanIndexBuffer::VulkanIndexBuffer(const VkDeviceSize &numIndices)
    : VulkanBuffer(numIndices * sizeof(uint32_t),
                   VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
                   VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT),
      m_StagingBuffer(numIndices * sizeof(uint32_t),
                      VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                      VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT) {
}
VulkanIndexBuffer::VulkanIndexBuffer(VulkanIndexBuffer &&o) noexcept
    : VulkanBuffer(std::forward<VulkanIndexBuffer>(o)),
      m_StagingBuffer(std::forward<VulkanBuffer>(o.m_StagingBuffer)),
      m_NumIndices(o.m_NumIndices) {
  o.m_NumIndices = 0;
}
VulkanIndexBuffer &VulkanIndexBuffer::operator=(VulkanIndexBuffer &&o) noexcept {
  VulkanBuffer::operator=(std::forward<VulkanIndexBuffer>(o));
  m_StagingBuffer = std::forward<VulkanBuffer>(o.m_StagingBuffer);
  m_NumIndices = o.m_NumIndices;
  o.m_NumIndices = 0;

  return *this;
}

bool VulkanIndexBuffer::write(const VulkanCommandBuffer &commandBuffer, size_t numIndices, const uint32_t *indexData) {
  size_t dataSize = numIndices * sizeof(uint32_t);
  dataSize = std::min(dataSize, static_cast<size_t>(size()));

  void *data = m_StagingBuffer.map();
  memcpy(data, indexData, dataSize);
  m_StagingBuffer.unmap();

  if (VulkanBuffer::copyBuffer(m_StagingBuffer, *this, commandBuffer)) {
    m_NumIndices = dataSize / sizeof(uint32_t);
    return true;
  }
  // TODO: Change the number of vertices? -> 0
  return false;
}

void VulkanIndexBuffer::cleanup() {
  m_StagingBuffer.cleanup();
  VulkanBuffer::cleanup();
}

} // namespace Froth
