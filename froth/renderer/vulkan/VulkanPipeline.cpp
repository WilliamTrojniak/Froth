#include "VulkanPipeline.h"
#include "core/logger/Logger.h"
#include "renderer/vulkan/VulkanDevice.h"
#include "renderer/vulkan/VulkanPipelineLayout.h"
#include "renderer/vulkan/VulkanRenderPass.h"
#include "vulkan/vulkan_core.h"
#include <vector>

namespace Froth {

VulkanPipeline::VulkanPipeline(const VulkanDevice &device,
                               const VulkanPipelineLayout &pipelineLayout,
                               const VulkanRenderPass &renderPass,
                               const std::vector<VkPipelineShaderStageCreateInfo> &shaderStages,
                               const VkPipelineVertexInputStateCreateInfo &vertexInputInfo,
                               const VkPipelineInputAssemblyStateCreateInfo &inputAssemblyInfo,
                               const VkViewport &viewport,
                               const VkRect2D &scissor,
                               const VkPipelineRasterizationStateCreateInfo &rasterizerInfo,
                               const VkPipelineMultisampleStateCreateInfo &multisampleInfo,
                               const VkPipelineDepthStencilStateCreateInfo &depthStencilInfo,
                               const VkPipelineColorBlendStateCreateInfo &colorBlendInfo,
                               const VkPipelineDynamicStateCreateInfo &dynamicState)
    : m_Device(device) {

  // TODO: Support multiple viewports and scissors
  VkPipelineViewportStateCreateInfo viewportStateInfo{};
  viewportStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
  viewportStateInfo.viewportCount = 1;
  viewportStateInfo.pViewports = &viewport;
  viewportStateInfo.scissorCount = 1;
  viewportStateInfo.pScissors = &scissor;

  VkGraphicsPipelineCreateInfo pipelineInfo{};
  pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
  pipelineInfo.stageCount = static_cast<uint32_t>(shaderStages.size());
  pipelineInfo.pStages = shaderStages.data();
  pipelineInfo.pVertexInputState = &vertexInputInfo;
  pipelineInfo.pInputAssemblyState = &inputAssemblyInfo;
  pipelineInfo.pViewportState = &viewportStateInfo;
  pipelineInfo.pRasterizationState = &rasterizerInfo;
  pipelineInfo.pMultisampleState = &multisampleInfo;
  pipelineInfo.pDepthStencilState = &depthStencilInfo;
  pipelineInfo.pColorBlendState = &colorBlendInfo;
  pipelineInfo.pDynamicState = &dynamicState;
  pipelineInfo.layout = pipelineLayout;
  pipelineInfo.renderPass = renderPass;
  pipelineInfo.subpass = 0;
  pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
  pipelineInfo.basePipelineIndex = -1;

  // TODO: Support pipeline cache
  // TODO: Support multiple pipelines?
  if (vkCreateGraphicsPipelines(m_Device, VK_NULL_HANDLE, 1, &pipelineInfo, m_Device.instance().allocator(), &m_Pipeline) != VK_SUCCESS) {
    FROTH_ERROR("Failed to create Vulkan Pipeline")
  }
}

VulkanPipeline::~VulkanPipeline() {
  cleanup();
}

void VulkanPipeline::cleanup() {
  if (m_Pipeline) {
    vkDestroyPipeline(m_Device, m_Pipeline, m_Device.instance().allocator());
    m_Pipeline = nullptr;
    FROTH_DEBUG("Destroyed Vulkan Pipeline")
  }
}

} // namespace Froth
