#include "VulkanCommandBuffer.h"
#include "src/core/logger/Logger.h"
#include "src/renderer/vulkan/VulkanContext.h"

namespace Froth {

VulkanCommandBuffer::~VulkanCommandBuffer() {
  if (m_Buffer) {
    FROTH_WARN("Command buffer not freed")
  }
}

VulkanCommandBuffer::VulkanCommandBuffer(VulkanCommandBuffer &&o)
    : m_Buffer(o.m_Buffer) {
  o.m_Buffer = nullptr;
}

bool VulkanCommandBuffer::reset() {
  if (vkResetCommandBuffer(m_Buffer, 0) != VK_SUCCESS) {
    FROTH_WARN("Failed to reset command buffer");
    return false;
  }
  return true;
}

void VulkanCommandBuffer::cleanup(VkCommandPool pool) {
  if (m_Buffer) {
    vkFreeCommandBuffers(VulkanContext::get().device(), pool, 1, &m_Buffer);
    m_Buffer = nullptr;
    FROTH_DEBUG("Destroyed Vulkan Command Buffer");
  }
}

} // namespace Froth
