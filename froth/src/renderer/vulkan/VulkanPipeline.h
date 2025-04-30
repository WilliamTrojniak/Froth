#pragma once

#include "src/renderer/vulkan/VulkanDevice.h"
#include "src/renderer/vulkan/VulkanPipelineLayout.h"
#include "src/renderer/vulkan/VulkanRenderPass.h"

namespace Froth {

class VulkanPipeline {
public:
  VulkanPipeline(const VulkanDevice &device,
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
                 const VkPipelineDynamicStateCreateInfo &dynamicState);
  ~VulkanPipeline();

  VulkanPipeline(VulkanPipeline const &) = delete;
  VulkanPipeline &operator=(VulkanPipeline const &) = delete;

  operator VkPipeline() const { return m_Pipeline; }

private:
  const VulkanDevice &m_Device;
  VkPipeline m_Pipeline;

  void cleanup();
};

} // namespace Froth
