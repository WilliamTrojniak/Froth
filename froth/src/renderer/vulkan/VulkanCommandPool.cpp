#include "VulkanCommandPool.h"
#include "VulkanRenderer.h"
#include "src/core/logger/Logger.h"

namespace Froth {

VulkanCommandPool::VulkanCommandPool(uint32_t queueFamilyIndex) {
  VkCommandPoolCreateInfo poolInfo{};
  poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
  poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
  poolInfo.queueFamilyIndex = queueFamilyIndex;

  if (vkCreateCommandPool(VulkanRenderer::context().device, &poolInfo, VulkanRenderer::context().instance.allocator(), &m_Pool) != VK_SUCCESS) {
    FROTH_ERROR("Failed to create Vulkan Command Pool");
  }
}

VulkanCommandPool::~VulkanCommandPool() {
  cleanup();
}

void VulkanCommandPool::cleanup() {
  if (m_Pool) {
    vkDestroyCommandPool(VulkanRenderer::context().device, m_Pool, VulkanRenderer::context().instance.allocator());
    m_Pool = nullptr;
    FROTH_DEBUG("Destroyed Vulkan Command Pool");
  }
}

} // namespace Froth
