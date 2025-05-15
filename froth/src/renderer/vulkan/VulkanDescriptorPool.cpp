#include "VulkanDescriptorPool.h"
#include "src/core/logger/Logger.h"
#include "src/renderer/vulkan/VulkanContext.h"
#include <array>
#include <vulkan/vulkan_core.h>

namespace Froth {

VulkanDescriptorPool::VulkanDescriptorPool(uint32_t maxSets, uint32_t maxUniformBuffers, uint32_t maxImageSamplers) {
  std::array<VkDescriptorPoolSize, 2> poolSizes{};
  poolSizes[0].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
  poolSizes[0].descriptorCount = maxUniformBuffers;
  poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
  poolSizes[1].descriptorCount = maxImageSamplers;

  VkDescriptorPoolCreateInfo createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
  createInfo.maxSets = maxSets;
  createInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
  createInfo.pPoolSizes = poolSizes.data();

  VulkanContext &vctx = VulkanContext::get();
  if (vkCreateDescriptorPool(vctx.device(), &createInfo, vctx.allocator(), &m_Pool) != VK_SUCCESS) {
    FROTH_ERROR("Failed to create Vulkan Descriptor Pool");
  }
}

VulkanDescriptorPool::VulkanDescriptorPool(VulkanDescriptorPool &&o) noexcept
    : m_Pool(o.m_Pool) {
  o.m_Pool = nullptr;
}

VulkanDescriptorPool &VulkanDescriptorPool::operator=(VulkanDescriptorPool &&o) noexcept {
  m_Pool = o.m_Pool;
  o.m_Pool = nullptr;

  return *this;
}

void VulkanDescriptorPool::cleanup() {
  if (m_Pool) {
    VulkanContext &vctx = VulkanContext::get();
    vkDestroyDescriptorPool(vctx.device(), m_Pool, vctx.allocator());
    m_Pool = nullptr;
    FROTH_DEBUG("Destroyed Vulkan Descriptor Pool", m_Pool);
  }
}

VulkanDescriptorPool::~VulkanDescriptorPool() {
  cleanup();
}

} // namespace Froth
