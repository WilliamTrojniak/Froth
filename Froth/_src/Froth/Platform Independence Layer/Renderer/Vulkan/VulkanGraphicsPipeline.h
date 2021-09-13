#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "VulkanDevice.h"
#include "VulkanPipelineLayout.h"
#include "VulkanRenderPass.h"
#include "VulkanShaderModule.h"

namespace Froth
{
	class VulkanGraphicsPipeline
	{
	public:
		VulkanGraphicsPipeline() = default;
		VulkanGraphicsPipeline(const VulkanDevice& device,
			U32 stageCount, const VkPipelineShaderStageCreateInfo* shaderStages,
			const VkPipelineVertexInputStateCreateInfo* vertexInputState,
			const VkPipelineInputAssemblyStateCreateInfo* inputAssemblyState,
			const VkPipelineTessellationStateCreateInfo* tesselationState,
			const VkPipelineViewportStateCreateInfo* viewportState,
			const VkPipelineRasterizationStateCreateInfo* rasterizationState,
			const VkPipelineMultisampleStateCreateInfo* multisampleState,
			const VkPipelineDepthStencilStateCreateInfo* depthStencilState,
			const VkPipelineColorBlendStateCreateInfo* colorBlendState,
			const VkPipelineDynamicStateCreateInfo* dynamicState,
			const VulkanPipelineLayout& pipelineLayout,
			const VulkanRenderPass& renderPass,
			U32 subpass, VkPipeline basePipelineHandle, U32 basePipelineIndex);
		VulkanGraphicsPipeline(VulkanGraphicsPipeline& other) = delete;
		VulkanGraphicsPipeline(VulkanGraphicsPipeline&& other);
		~VulkanGraphicsPipeline();

		VulkanGraphicsPipeline& operator=(VulkanGraphicsPipeline&& other);

		const VkPipeline& getPipeline() const { return m_Pipeline; }

		static VkPipelineShaderStageCreateInfo createShaderStageInfo(const VkShaderStageFlagBits& stage, const VulkanShaderModule& module);
		static VkVertexInputBindingDescription createVertexInputBindingDescription(U32 binding, U32 vertexSize);
		static VkVertexInputAttributeDescription createVertexInputAttributeDescription(U32 location, U32 binding, VkFormat format, U32 offset);
		static VkPipelineVertexInputStateCreateInfo createVertexInputStateInfo(U32 vertexBindingDescCount, const VkVertexInputBindingDescription* vertexBindingDescs, U32 attributeDescCount, const VkVertexInputAttributeDescription* attributeDescs);
		static VkPipelineInputAssemblyStateCreateInfo createInputAssemblyStateInfo();
		static VkPipelineTessellationStateCreateInfo createTesselationStateInfo(U32 patchControlPoints);
		static VkPipelineViewportStateCreateInfo createViewportStateInfo(U32 viewportCount, const VkViewport* viewports, U32 scissorCount, const VkRect2D* scissors);
		static VkPipelineRasterizationStateCreateInfo createRasterizationStateInfo(VkBool32 depthClampEnable, VkBool32 rasterizerDiscardEnable, const VkPolygonMode& polygonMode, const VkCullModeFlags& cullMode, VkBool32 depthBiasEnable, F32 depthBiasConstantFactor = 0.0f, F32 depthBiasClamp = 0.0f, F32 depthBiasSlopeFactor = 0.0f);
		static VkPipelineMultisampleStateCreateInfo createMultisampleStateInfo(const VkSampleCountFlagBits& rasterizationSamples = VK_SAMPLE_COUNT_1_BIT, VkBool32 sampleShadingEnable = VK_FALSE, F32 minSampleShading = 1.0f, const VkSampleMask* sampleMask = nullptr, VkBool32 alphaToCoverageEnable = VK_FALSE, VkBool32 alphaToOneEnable = VK_FALSE);
		static VkPipelineDepthStencilStateCreateInfo createDepthStencilStateInfo(VkBool32 depthTestEnable, VkBool32 depthWriteEnable, const VkCompareOp& depthCompareOp, VkBool32 depthBoundsTestEnable, VkBool32 stencilTestEnable, const VkStencilOpState& front, const VkStencilOpState& back, F32 minDepthBounds, F32 maxDepthBounds);
		static VkPipelineColorBlendAttachmentState createColorBlendAttachmentStateInfo(VkBool32 blendEnable, const VkBlendFactor& srcColorBlendFactor, const VkBlendFactor& dstColorBlendFactor, const VkBlendOp& colorBlendOp, const VkBlendFactor& srcAlphaBlendFactor, const VkBlendFactor& dstAlphaBlendFactor, const VkBlendOp& alphaBlendOp, const VkColorComponentFlags& colorWriteMask);
		static VkPipelineColorBlendStateCreateInfo createColorBlendStateInfo(VkBool32 logicOpEnable, const VkLogicOp& logicOp, U32 attachmentCount, const VkPipelineColorBlendAttachmentState* attachments, F32* blendConstants);
		static VkPipelineDynamicStateCreateInfo createDynamicStateInfo(U32 dynamicStateCount, const VkDynamicState* dynamicStates);

	private:
		const VulkanDevice* m_Device;
		VkPipeline m_Pipeline;
	};
}
