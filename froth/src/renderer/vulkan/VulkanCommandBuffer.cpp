#include "VulkanCommandBuffer.h"
#include "VulkanRenderer.h"
#include "src/core/logger/Logger.h"

namespace Froth {

VulkanCommandBuffer::VulkanCommandBuffer(const VulkanCommandPool &pool)
    : m_Pool(pool) {
  VkCommandBufferAllocateInfo allocInfo{};
  allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  allocInfo.commandPool = m_Pool;
  allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  allocInfo.commandBufferCount = 1;

  if (vkAllocateCommandBuffers(VulkanRenderer::context().device, &allocInfo, &m_Buffer) != VK_SUCCESS) {
    FROTH_ERROR("Failed to allocate command buffer");
  }
}

VulkanCommandBuffer::~VulkanCommandBuffer() {
  cleanup();
}

VulkanCommandBuffer::VulkanCommandBuffer(VulkanCommandBuffer &&other)
    : m_Pool(other.m_Pool) {
  m_Buffer = other.m_Buffer;
  other.m_Buffer = nullptr;
}

void VulkanCommandBuffer::cleanup() {
  if (m_Buffer) {
    vkFreeCommandBuffers(VulkanRenderer::context().device, m_Pool, 1, &m_Buffer);
    FROTH_DEBUG("Destroyed Vulkan Command Buffer");
  }
}

} // namespace Froth
