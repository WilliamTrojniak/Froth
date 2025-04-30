#include "VulkanSemaphore.h"
#include "src/core/logger/Logger.h"

namespace Froth {

VulkanSemaphore::VulkanSemaphore(const VulkanDevice &device)
    : m_Device(device) {
  VkSemaphoreCreateInfo semaphoreInfo{};
  semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

  if (vkCreateSemaphore(device, &semaphoreInfo, m_Device.instance().allocator(), &m_Semaphore) != VK_SUCCESS) {
    FROTH_ERROR("Failed to create semaphore");
  }
}

VulkanSemaphore::VulkanSemaphore(VulkanSemaphore &&other)
    : m_Device(other.m_Device) {
  m_Semaphore = other.m_Semaphore;
  other.m_Semaphore = nullptr;
}

VulkanSemaphore::~VulkanSemaphore() {
  cleanup();
}

void VulkanSemaphore::cleanup() {
  if (m_Semaphore) {
    vkDestroySemaphore(m_Device, m_Semaphore, m_Device.instance().allocator());
    m_Semaphore = nullptr;
    FROTH_DEBUG("Destroyed Vulkan Semaphore")
  }
}

} // namespace Froth
