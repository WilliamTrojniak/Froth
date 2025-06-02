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

bool VulkanCommandBuffer::beginSingleTime() {
  VkCommandBufferBeginInfo beginInfo{};
  beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
  beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
  if (vkBeginCommandBuffer(m_Buffer, &beginInfo) != VK_SUCCESS) {
    FROTH_WARN("Failed to begin single time command buffer")
    return false;
  }
  return true;
};

bool VulkanCommandBuffer::end() {
  if (vkEndCommandBuffer(m_Buffer) != VK_SUCCESS) {
    FROTH_WARN("Failed to end command buffer")
    return false;
  }
  return true;
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
