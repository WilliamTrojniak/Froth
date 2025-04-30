
#include "VulkanBuffer.h"
#include "VulkanCommandBuffer.h"
#include "VulkanDevice.h"
#include "src/core/logger/Logger.h"
#include "src/renderer/vulkan/VulkanRenderer.h"

namespace Froth {

VulkanBuffer::VulkanBuffer(const VkDeviceSize &size, VkBufferUsageFlags usage, VkMemoryPropertyFlags memProperties) {

  VkBufferCreateInfo bufferInfo{};
  bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
  bufferInfo.size = size;
  bufferInfo.usage = usage;
  bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

  if (vkCreateBuffer(VulkanRenderer::context().device, &bufferInfo, VulkanRenderer::context().instance.allocator(), &m_Buffer) != VK_SUCCESS) {
    FROTH_ERROR("Failed to create vertex buffer");
  }
  m_Size = size;

  VkMemoryRequirements memRequirements;
  vkGetBufferMemoryRequirements(VulkanRenderer::context().device, m_Buffer, &memRequirements);
  m_Memory = VulkanRenderer::context().device.allocateMemory(memRequirements, memProperties);

  if (vkBindBufferMemory(VulkanRenderer::context().device, m_Buffer, m_Memory, 0) != VK_SUCCESS) {
    FROTH_ERROR("Failed to bind buffer memory");
    // FIXME: Improper cleanup
  }
}

VulkanBuffer::VulkanBuffer(VulkanBuffer &&other) noexcept
    : m_Buffer(other.m_Buffer), m_Size(other.m_Size), m_Memory(other.m_Memory) {
  other.m_Buffer = nullptr;
  other.m_Size = 0;
  other.m_Memory = nullptr;
}

void *VulkanBuffer::map() const {
  void *data;
  vkMapMemory(VulkanRenderer::context().device, m_Memory, 0, m_Size, 0, &data);
  return data;
}

void VulkanBuffer::unmap() const {
  vkUnmapMemory(VulkanRenderer::context().device, m_Memory);
}

VulkanBuffer::~VulkanBuffer() {
  cleanup();
}

void VulkanBuffer::cleanup() {
  if (m_Memory) {
    vkFreeMemory(VulkanRenderer::context().device, m_Memory, VulkanRenderer::context().instance.allocator());
    m_Memory = nullptr;
    FROTH_DEBUG("Freed Vulkan Buffer Memory");
  }

  if (m_Buffer) {
    vkDestroyBuffer(VulkanRenderer::context().device, m_Buffer, VulkanRenderer::context().instance.allocator());
    m_Buffer = nullptr;
    FROTH_DEBUG("Destroyed Vulkan Buffer");
  }
}

void VulkanBuffer::copyBuffer(const VulkanBuffer &src, const VulkanBuffer &dest, const VulkanCommandPool &commandPool) {
  VulkanCommandBuffer commandBuffer(commandPool);

  VkCommandBufferBeginInfo beginInfo{};
  beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
  beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
  if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
    FROTH_ERROR("Failed to begin command buffer");
  }

  VkBufferCopy copyRegion{};
  copyRegion.srcOffset = 0;
  copyRegion.dstOffset = 0;
  copyRegion.size = std::min(src.size(), dest.size());
  vkCmdCopyBuffer(commandBuffer, src, dest, 1, &copyRegion);

  if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
    FROTH_ERROR("Failed to end command buffer");
  }

  VkSubmitInfo submitInfo{};
  VkCommandBuffer b = commandBuffer;
  submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
  submitInfo.commandBufferCount = 1;
  submitInfo.pCommandBuffers = &b;
  if (vkQueueSubmit(VulkanRenderer::context().device.getQueueFamilies().graphics.queue, 1, &submitInfo, VK_NULL_HANDLE) != VK_SUCCESS) {
    FROTH_ERROR("Failed to submit to queue");
  }

  if (vkQueueWaitIdle(VulkanRenderer::context().device.getQueueFamilies().graphics.queue) != VK_SUCCESS) {
    FROTH_ERROR("Failed to wait for queue idle");
  }
}

} // namespace Froth
