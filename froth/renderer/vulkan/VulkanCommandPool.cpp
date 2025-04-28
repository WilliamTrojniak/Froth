#include "VulkanCommandPool.h"
#include "core/logger/Logger.h"
#include "renderer/vulkan/VulkanDevice.h"
#include "vulkan/vulkan_core.h"

namespace Froth {

VulkanCommandPool::VulkanCommandPool(const VulkanDevice &device, uint32_t queueFamilyIndex)
    : m_Device(device) {
  VkCommandPoolCreateInfo poolInfo{};
  poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
  poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
  poolInfo.queueFamilyIndex = queueFamilyIndex;

  if (vkCreateCommandPool(m_Device, &poolInfo, nullptr, &m_Pool) != VK_SUCCESS) {
    FROTH_ERROR("Failed to create Vulkan Command Pool");
  }
}

VulkanCommandPool::~VulkanCommandPool() {
  cleanup();
}

void VulkanCommandPool::cleanup() {
  if (m_Pool) {
    vkDestroyCommandPool(m_Device, m_Pool, m_Device.instance().allocator());
    m_Pool = nullptr;
    FROTH_DEBUG("Destroyed Vulkan Command Pool");
  }
}

} // namespace Froth
