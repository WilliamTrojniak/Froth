#include "VulkanSemaphore.h"
#include "src/core/logger/Logger.h"
#include "src/renderer/vulkan/VulkanContext.h"

namespace Froth {

VulkanSemaphore::VulkanSemaphore() {
  VkSemaphoreCreateInfo semaphoreInfo{};
  semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

  VulkanContext &vctx = VulkanContext::get();
  if (vkCreateSemaphore(vctx.device(), &semaphoreInfo, vctx.allocator(), &m_Semaphore) != VK_SUCCESS) {
    FROTH_ERROR("Failed to create semaphore");
  }
}

VulkanSemaphore::VulkanSemaphore(VulkanSemaphore &&other)
    : m_Semaphore(other.m_Semaphore) {
  other.m_Semaphore = nullptr;
}

VulkanSemaphore::~VulkanSemaphore() {
  cleanup();
}

void VulkanSemaphore::cleanup() {
  if (m_Semaphore) {
    VulkanContext &vctx = VulkanContext::get();
    vkDestroySemaphore(vctx.device(), m_Semaphore, vctx.allocator());
    m_Semaphore = nullptr;
    FROTH_DEBUG("Destroyed Vulkan Semaphore")
  }
}

} // namespace Froth
