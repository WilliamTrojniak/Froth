#include "VulkanDescriptorSetLayout.h"
#include "src/core/logger/Logger.h"
#include "src/renderer/vulkan/VulkanContext.h"
#include <array>
#include <vector>

namespace Froth {

VulkanDescriptorSetLayout::VulkanDescriptorSetLayout(const std::vector<VkDescriptorSetLayoutBinding> &data) {
  // TODO: Customizable descriptor set layout
  // VkDescriptorSetLayoutBinding uboLayoutBinding{};
  // uboLayoutBinding.binding = 0;
  // uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
  // uboLayoutBinding.descriptorCount = 1;
  // uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
  // uboLayoutBinding.pImmutableSamplers = nullptr;

  auto props = VulkanContext::get().device().props();
  FROTH_INFO("Max descriptor count: %u", props.limits.maxDescriptorSetSampledImages);
  VkDescriptorSetLayoutBinding samplerLayoutBinding{};
  samplerLayoutBinding.binding = 0;
  samplerLayoutBinding.descriptorCount = 4;
  samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
  samplerLayoutBinding.pImmutableSamplers = VK_NULL_HANDLE;
  samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

  // std::array<VkDescriptorSetLayoutBinding, 2> bindings = {uboLayoutBinding, samplerLayoutBinding};
  std::array<VkDescriptorSetLayoutBinding, 1> bindings = {samplerLayoutBinding};

  VkDescriptorSetLayoutCreateInfo layoutInfo{};
  layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
  layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
  layoutInfo.pBindings = bindings.data();

  VulkanContext &vctx = VulkanContext::get();
  if (vkCreateDescriptorSetLayout(vctx.device(), &layoutInfo, vctx.allocator(), &m_DescriptorSetLayout) != VK_SUCCESS) {
    FROTH_ERROR("Failed to create Descriptor Set Layout")
  }
}

VulkanDescriptorSetLayout::VulkanDescriptorSetLayout(VulkanDescriptorSetLayout &&o) noexcept
    : m_DescriptorSetLayout(o.m_DescriptorSetLayout) {
  o.m_DescriptorSetLayout = nullptr;
}

VulkanDescriptorSetLayout &VulkanDescriptorSetLayout::operator=(VulkanDescriptorSetLayout &&o) noexcept {
  m_DescriptorSetLayout = o.m_DescriptorSetLayout;
  o.m_DescriptorSetLayout = nullptr;

  return *this;
}

VulkanDescriptorSetLayout::~VulkanDescriptorSetLayout() {
  cleanup();
}

void VulkanDescriptorSetLayout::cleanup() {
  if (m_DescriptorSetLayout) {
    VulkanContext &vctx = VulkanContext::get();
    vkDestroyDescriptorSetLayout(vctx.device(), m_DescriptorSetLayout, vctx.allocator());
    m_DescriptorSetLayout = nullptr;
    FROTH_DEBUG("Destroyed Vulkan Descriptor Set Layout")
  }
}

} // namespace Froth
