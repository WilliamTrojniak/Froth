#include "VulkanDescriptorSetLayout.h"
#include "VulkanDevice.h"
#include "src/core/logger/Logger.h"
#include <array>

namespace Froth {

VulkanDescriptorSetLayout::VulkanDescriptorSetLayout(const VulkanDevice &device)
    : m_Device(device) {
  // TODO: Customizable descriptor set layout
  VkDescriptorSetLayoutBinding uboLayoutBinding{};
  uboLayoutBinding.binding = 0;
  uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
  uboLayoutBinding.descriptorCount = 1;
  uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
  uboLayoutBinding.pImmutableSamplers = nullptr;

  VkDescriptorSetLayoutBinding samplerLayoutBinding{};
  samplerLayoutBinding.binding = 1;
  samplerLayoutBinding.descriptorCount = 1;
  samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
  samplerLayoutBinding.pImmutableSamplers = nullptr;
  samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

  std::array<VkDescriptorSetLayoutBinding, 2> bindings = {uboLayoutBinding, samplerLayoutBinding};

  VkDescriptorSetLayoutCreateInfo layoutInfo{};
  layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
  layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
  layoutInfo.pBindings = bindings.data();

  if (vkCreateDescriptorSetLayout(device, &layoutInfo, device.instance().allocator(), &m_DescriptorSetLayout) != VK_SUCCESS) {
    FROTH_ERROR("Failed to create Descriptor Set Layout")
  }
}

VulkanDescriptorSetLayout::~VulkanDescriptorSetLayout() {
  cleanup();
}

void VulkanDescriptorSetLayout::cleanup() {
  if (m_DescriptorSetLayout) {
    vkDestroyDescriptorSetLayout(m_Device, m_DescriptorSetLayout, m_Device.instance().allocator());
    m_DescriptorSetLayout = nullptr;
    FROTH_DEBUG("Destroyed Vulkan Descriptor Set Layout")
  }
}

} // namespace Froth
