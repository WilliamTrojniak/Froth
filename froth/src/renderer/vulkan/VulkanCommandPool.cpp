#include "VulkanCommandPool.h"
#include "src/core/logger/Logger.h"
#include "src/renderer/vulkan/VulkanCommandBuffer.h"
#include "src/renderer/vulkan/VulkanContext.h"

namespace Froth {

VulkanCommandPool::VulkanCommandPool(uint32_t queueFamilyIndex) {
  VkCommandPoolCreateInfo poolInfo{};
  poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
  poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
  poolInfo.queueFamilyIndex = queueFamilyIndex;

  VulkanContext &vctx = VulkanContext::get();
  if (vkCreateCommandPool(vctx.device(), &poolInfo, vctx.allocator(), &m_Pool) != VK_SUCCESS) {
    FROTH_ERROR("Failed to create Vulkan Command Pool");
  }
}

VulkanCommandPool::VulkanCommandPool(VulkanCommandPool &&o) noexcept
    : m_Pool(o.m_Pool) {
  o.m_Pool = nullptr;
}

VulkanCommandPool &VulkanCommandPool::operator=(VulkanCommandPool &&o) noexcept {
  m_Pool = o.m_Pool;
  o.m_Pool = nullptr;
  return *this;
}

VulkanCommandPool::~VulkanCommandPool() {
  cleanup();
}

VulkanCommandBuffer VulkanCommandPool::AllocateCommandBuffer() const {
  VulkanCommandBuffer buffer;

  VkCommandBufferAllocateInfo allocInfo{};
  allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  allocInfo.commandPool = m_Pool;
  allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  allocInfo.commandBufferCount = 1;

  if (vkAllocateCommandBuffers(VulkanContext::get().device(), &allocInfo, &buffer.data()) != VK_SUCCESS) {
    FROTH_ERROR("Failed to allocate command buffer");
  }

  return buffer;
}

void VulkanCommandPool::cleanup() {
  if (m_Pool) {
    VulkanContext &vctx = VulkanContext::get();
    vkDestroyCommandPool(vctx.device(), m_Pool, vctx.allocator());
    m_Pool = nullptr;
    FROTH_DEBUG("Destroyed Vulkan Command Pool");
  }
}

} // namespace Froth
