#pragma once

#include "renderer/vulkan/VulkanCommandPool.h"
#include "renderer/vulkan/VulkanDevice.h"
#include "vulkan/vulkan_core.h"
namespace Froth {

class VulkanCommandBuffer {
public:
  VulkanCommandBuffer(const VulkanDevice &device, const VulkanCommandPool &pool);
  ~VulkanCommandBuffer();

  VulkanCommandBuffer(VulkanCommandBuffer const &) = delete;
  VulkanCommandBuffer &operator=(VulkanCommandBuffer const &) = delete;

  VulkanCommandBuffer(VulkanCommandBuffer &&);
  operator VkCommandBuffer() const { return m_Buffer; }

private:
  const VulkanDevice &m_Device;
  const VulkanCommandPool &m_Pool;
  VkCommandBuffer m_Buffer;

  void cleanup();
};

} // namespace Froth
