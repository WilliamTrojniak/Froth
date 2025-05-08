#include "VulkanPipelineLayout.h"
#include "glm/ext/matrix_float4x4.hpp"
#include "src/core/logger/Logger.h"
#include "src/renderer/vulkan/VulkanContext.h"
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

  VulkanContext &vctx = VulkanContext::get();
  if (vkCreatePipelineLayout(vctx.device(), &layoutInfo, vctx.allocator(), &m_Layout) != VK_SUCCESS) {
    FROTH_ERROR("Failed to create pipeline layout");
  }
}

VulkanPipelineLayout::~VulkanPipelineLayout() {
  cleanup();
}

void VulkanPipelineLayout::cleanup() {
  if (m_Layout) {
    VulkanContext &vctx = VulkanContext::get();
    vkDestroyPipelineLayout(vctx.device(), m_Layout, vctx.allocator());
    m_Layout = nullptr;
    FROTH_DEBUG("Destroyed Vulkan Pipeline Layout")
  }
}

} // namespace Froth
