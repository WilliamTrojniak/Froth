#include "VulkanSemaphore.h"
#include "VulkanRenderer.h"
#include "src/core/logger/Logger.h"

namespace Froth {

VulkanSemaphore::VulkanSemaphore() {
  VkSemaphoreCreateInfo semaphoreInfo{};
  semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

  if (vkCreateSemaphore(VulkanRenderer::context().device, &semaphoreInfo, VulkanRenderer::context().instance.allocator(), &m_Semaphore) != VK_SUCCESS) {
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
    vkDestroySemaphore(VulkanRenderer::context().device, m_Semaphore, VulkanRenderer::context().instance.allocator());
    m_Semaphore = nullptr;
    FROTH_DEBUG("Destroyed Vulkan Semaphore")
  }
}

} // namespace Froth
