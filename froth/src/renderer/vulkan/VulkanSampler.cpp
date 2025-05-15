#include "VulkanSampler.h"
#include "src/core/logger/Logger.h"
#include "src/renderer/vulkan/VulkanContext.h"
#include <vulkan/vulkan_core.h>

namespace Froth {

VulkanSampler::VulkanSampler(const VkSamplerCreateInfo &info) {

  VulkanContext &vctx = VulkanContext::get();
  if (vkCreateSampler(vctx.device(), &info, vctx.allocator(), &m_Sampler) != VK_SUCCESS) {
    FROTH_ERROR("Failed to create Vulkan Sampler")
  }
}

VulkanSampler::VulkanSampler(VulkanSampler &&o) noexcept
    : m_Sampler(o.m_Sampler) {
  o.m_Sampler = nullptr;
}

VulkanSampler &VulkanSampler::operator=(VulkanSampler &&o) noexcept {
  m_Sampler = o.m_Sampler;
  o.m_Sampler = nullptr;

  return *this;
}

void VulkanSampler::cleanup() {
  if (m_Sampler) {
    VulkanContext &vctx = VulkanContext::get();
    vkDestroySampler(vctx.device(), m_Sampler, vctx.allocator());
    m_Sampler = nullptr;
    FROTH_DEBUG("Destroyed Vulkan sampler");
  }
}

VulkanSampler::Builder::Builder()
    : m_Info({
          .sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
          .magFilter = VK_FILTER_LINEAR,
          .minFilter = VK_FILTER_LINEAR,
          .mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR,
          .addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT,
          .addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT,
          .addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT,
          .anisotropyEnable = VK_FALSE,
          .compareEnable = VK_FALSE,
          .borderColor = VK_BORDER_COLOR_INT_OPAQUE_WHITE,
          .unnormalizedCoordinates = VK_FALSE,
      }) {
}

VulkanSampler VulkanSampler::Builder::build() {
  return VulkanSampler(m_Info);
}

} // namespace Froth
