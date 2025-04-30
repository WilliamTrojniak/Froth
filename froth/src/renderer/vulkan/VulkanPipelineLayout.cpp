#include "VulkanPipelineLayout.h"
#include "VulkanRenderer.h"
#include "src/core/logger/Logger.h"
#include <vector>

namespace Froth {

VulkanPipelineLayout::VulkanPipelineLayout(const std::vector<VkDescriptorSetLayout> &descSetLayouts) {

  VkPipelineLayoutCreateInfo layoutInfo{};
  layoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
  layoutInfo.setLayoutCount = static_cast<uint32_t>(descSetLayouts.size());
  layoutInfo.pSetLayouts = descSetLayouts.data();
  layoutInfo.pushConstantRangeCount = 0;
  layoutInfo.pPushConstantRanges = nullptr;

  if (vkCreatePipelineLayout(VulkanRenderer::context().device, &layoutInfo, VulkanRenderer::context().instance.allocator(), &m_Layout) != VK_SUCCESS) {
    FROTH_ERROR("Failed to create pipeline layout");
  }
}

VulkanPipelineLayout::~VulkanPipelineLayout() {
  cleanup();
}

void VulkanPipelineLayout::cleanup() {
  if (m_Layout) {
    vkDestroyPipelineLayout(VulkanRenderer::context().device, m_Layout, VulkanRenderer::context().instance.allocator());
    m_Layout = nullptr;
    FROTH_DEBUG("Destroyed Vulkan Pipeline Layout")
  }
}

} // namespace Froth
