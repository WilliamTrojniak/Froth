#include "VulkanFence.h"
#include "VulkanRenderer.h"
#include "src/core/logger/Logger.h"

namespace Froth {

VulkanFence::VulkanFence(bool signaled) {
  VkFenceCreateInfo semaphoreInfo{};
  semaphoreInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
  if (signaled) {
    semaphoreInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
  }

  if (vkCreateFence(VulkanRenderer::context().device, &semaphoreInfo, VulkanRenderer::context().instance.allocator(), &m_Fence) != VK_SUCCESS) {
    FROTH_ERROR("Failed to create Fence");
  }
}

VulkanFence::VulkanFence(VulkanFence &&other)
    : m_Fence(other.m_Fence) {
  other.m_Fence = nullptr;
}

VulkanFence::~VulkanFence() {
  cleanup();
}

void VulkanFence::cleanup() {
  if (m_Fence) {
    vkDestroyFence(VulkanRenderer::context().device, m_Fence, VulkanRenderer::context().instance.allocator());
    m_Fence = nullptr;
    FROTH_DEBUG("Destroyed Vulkan Fence")
  }
}

} // namespace Froth
