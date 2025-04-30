#include "VulkanPipelineLayout.h"
#include "VulkanRenderer.h"
#include "glm/ext/matrix_float4x4.hpp"
#include "src/core/logger/Logger.h"
#include "vulkan/vulkan_core.h"
#include <vector>

namespace Froth {

VulkanPipelineLayout::VulkanPipelineLayout(const std::vector<VkDescriptorSetLayout> &descSetLayouts) {

  // TODO: Configurable push constants?
  VkPushConstantRange pushConstantInfo{};
  pushConstantInfo.offset = 0;
  pushConstantInfo.size = sizeof(glm::mat4);
  pushConstantInfo.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

  VkPipelineLayoutCreateInfo layoutInfo{};
  layoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
  layoutInfo.setLayoutCount = static_cast<uint32_t>(descSetLayouts.size());
  layoutInfo.pSetLayouts = descSetLayouts.data();
  layoutInfo.pushConstantRangeCount = 1;
  layoutInfo.pPushConstantRanges = &pushConstantInfo;

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
