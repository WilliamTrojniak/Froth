#include "VulkanFence.h"
#include "src/core/logger/Logger.h"
#include "src/renderer/vulkan/VulkanContext.h"

namespace Froth {

VulkanFence::VulkanFence(bool signaled) {
  VkFenceCreateInfo semaphoreInfo{};
  semaphoreInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
  if (signaled) {
    semaphoreInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
  }

  VulkanContext &vctx = VulkanContext::get();
  if (vkCreateFence(vctx.device(), &semaphoreInfo, vctx.allocator(), &m_Fence) != VK_SUCCESS) {
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
    VulkanContext &vctx = VulkanContext::get();
    vkDestroyFence(vctx.device(), m_Fence, vctx.allocator());
    m_Fence = nullptr;
    FROTH_DEBUG("Destroyed Vulkan Fence")
  }
}

} // namespace Froth
