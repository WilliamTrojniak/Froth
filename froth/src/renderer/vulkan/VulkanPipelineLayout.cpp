#include "VulkanPipelineLayout.h"
#include "src/core/logger/Logger.h"
#include <vector>

namespace Froth {

VulkanPipelineLayout::VulkanPipelineLayout(const VulkanDevice &device, const std::vector<VkDescriptorSetLayout> &descSetLayouts)
    : m_Device(device) {

  VkPipelineLayoutCreateInfo layoutInfo{};
  layoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
  layoutInfo.setLayoutCount = static_cast<uint32_t>(descSetLayouts.size());
  layoutInfo.pSetLayouts = descSetLayouts.data();
  layoutInfo.pushConstantRangeCount = 0;
  layoutInfo.pPushConstantRanges = nullptr;

  if (vkCreatePipelineLayout(m_Device, &layoutInfo, m_Device.instance().allocator(), &m_Layout) != VK_SUCCESS) {
    FROTH_ERROR("Failed to create pipeline layout");
  }
}

VulkanPipelineLayout::~VulkanPipelineLayout() {
  cleanup();
}

void VulkanPipelineLayout::cleanup() {
  if (m_Layout) {
    vkDestroyPipelineLayout(m_Device, m_Layout, m_Device.instance().allocator());
    m_Layout = nullptr;
    FROTH_DEBUG("Destroyed Vulkan Pipeline Layout")
  }
}

} // namespace Froth
