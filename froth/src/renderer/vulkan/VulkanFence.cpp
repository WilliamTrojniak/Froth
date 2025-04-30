#include "VulkanFence.h"
#include "VulkanDevice.h"
#include "src/core/logger/Logger.h"

namespace Froth {

VulkanFence::VulkanFence(const VulkanDevice &device, bool signaled)
    : m_Device(device) {
  VkFenceCreateInfo semaphoreInfo{};
  semaphoreInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
  if (signaled) {
    semaphoreInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
  }

  if (vkCreateFence(device, &semaphoreInfo, m_Device.instance().allocator(), &m_Fence) != VK_SUCCESS) {
    FROTH_ERROR("Failed to create Fence");
  }
}

VulkanFence::VulkanFence(VulkanFence &&other)
    : m_Device(other.m_Device) {
  m_Fence = other.m_Fence;
  other.m_Fence = nullptr;
}

VulkanFence::~VulkanFence() {
  cleanup();
}

void VulkanFence::cleanup() {
  if (m_Fence) {
    vkDestroyFence(m_Device, m_Fence, m_Device.instance().allocator());
    m_Fence = nullptr;
    FROTH_DEBUG("Destroyed Vulkan Fence")
  }
}

} // namespace Froth
