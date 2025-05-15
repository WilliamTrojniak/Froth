#pragma once

#include <cstdint>
#include <vulkan/vulkan.h>

namespace Froth {
class VulkanDescriptorPool {
public:
  VulkanDescriptorPool() = default;
  VulkanDescriptorPool(uint32_t maxSets, uint32_t maxUniformBuffers, uint32_t maxImageSamplers);
  VulkanDescriptorPool(const VulkanDescriptorPool &) = delete;
  VulkanDescriptorPool &operator=(const VulkanDescriptorPool &) = delete;
  VulkanDescriptorPool(VulkanDescriptorPool &&) noexcept;
  VulkanDescriptorPool &operator=(VulkanDescriptorPool &&) noexcept;
  ~VulkanDescriptorPool();

  operator VkDescriptorPool() const { return m_Pool; }

  void cleanup();

private:
  VkDescriptorPool m_Pool = nullptr;
};

} // namespace Froth
