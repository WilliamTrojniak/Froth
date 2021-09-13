#include <frothpch.h>
#include "VulkanGraphicsPipeline.h"

namespace Froth
{

	VulkanGraphicsPipeline::VulkanGraphicsPipeline(
		const VulkanDevice& device,
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
		U32 subpass, VkPipeline basePipelineHandle, U32 basePipelineIndex)
		: m_Device(&device)
	{

		VkGraphicsPipelineCreateInfo graphicsPipelineInfo;
		graphicsPipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		graphicsPipelineInfo.pNext = nullptr;
		graphicsPipelineInfo.flags = 0;
		graphicsPipelineInfo.stageCount = stageCount;
		graphicsPipelineInfo.pStages = shaderStages;
		graphicsPipelineInfo.pVertexInputState = vertexInputState;
		graphicsPipelineInfo.pInputAssemblyState = inputAssemblyState;
		graphicsPipelineInfo.pTessellationState = tesselationState;
		graphicsPipelineInfo.pViewportState = viewportState;
		graphicsPipelineInfo.pRasterizationState = rasterizationState;
		graphicsPipelineInfo.pMultisampleState = multisampleState;
		graphicsPipelineInfo.pDepthStencilState = depthStencilState;
		graphicsPipelineInfo.pColorBlendState = colorBlendState;
		graphicsPipelineInfo.pDynamicState = dynamicState;
		graphicsPipelineInfo.layout = pipelineLayout.getLayout();
		graphicsPipelineInfo.renderPass = renderPass.getRenderPass();
		graphicsPipelineInfo.subpass = subpass;
		graphicsPipelineInfo.basePipelineHandle = basePipelineHandle;
		graphicsPipelineInfo.basePipelineIndex = basePipelineIndex;

		// TODO: Assert // TODO: Parameterize cache // TODO: Allow creation of multiple pipelines at once
		if (vkCreateGraphicsPipelines(m_Device->getDevice(), VK_NULL_HANDLE, 1, &graphicsPipelineInfo, nullptr, &m_Pipeline) != VK_SUCCESS) std::cout << "Failed to create graphics pipeline!" << std::endl;
	}

	VulkanGraphicsPipeline::VulkanGraphicsPipeline(VulkanGraphicsPipeline&& other)
		: m_Device(other.m_Device), m_Pipeline(other.m_Pipeline)
	{
		other.m_Pipeline = nullptr;
	}

	VulkanGraphicsPipeline::~VulkanGraphicsPipeline()
	{
		if (m_Pipeline != nullptr)
		{
			std::cout << "Pipeline destroyed" << std::endl;
			vkDestroyPipeline(m_Device->getDevice(), m_Pipeline, nullptr);
		}
	}

	VulkanGraphicsPipeline& VulkanGraphicsPipeline::operator=(VulkanGraphicsPipeline&& other)
	{
		m_Device = other.m_Device;
		m_Pipeline = other.m_Pipeline;

		other.m_Pipeline = nullptr;
		return *this;
	}

	VkPipelineShaderStageCreateInfo VulkanGraphicsPipeline::createShaderStageInfo(const VkShaderStageFlagBits& stage, const VulkanShaderModule& module)
	{
		return VkPipelineShaderStageCreateInfo{
			VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO, // sType
			nullptr, // pNext
			0, // flags
			stage,
			module.getModule(),
			"main", // Name of function to invoke in shader // TODO: Parameterize?
			nullptr // SpecializationInfo // TODO: Parameterize?
		};

	}

	VkVertexInputBindingDescription VulkanGraphicsPipeline::createVertexInputBindingDescription(U32 binding, U32 vertexSize)
	{
		return VkVertexInputBindingDescription{
			binding,
			vertexSize,
			VK_VERTEX_INPUT_RATE_VERTEX
		};
	}

	VkVertexInputAttributeDescription VulkanGraphicsPipeline::createVertexInputAttributeDescription(U32 location, U32 binding, VkFormat format, U32 offset)
	{
		return VkVertexInputAttributeDescription {
			location,
			binding,
			format,
			offset
		};
	}

	VkPipelineVertexInputStateCreateInfo VulkanGraphicsPipeline::createVertexInputStateInfo(U32 vertexBindingDescCount, const VkVertexInputBindingDescription* vertexBindingDescs, U32 attributeDescCount, const VkVertexInputAttributeDescription* attributeDescs)
	{
		return VkPipelineVertexInputStateCreateInfo{
			VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
			nullptr, // pNext
			0, // flags
			vertexBindingDescCount,
			vertexBindingDescs,
			attributeDescCount,
			attributeDescs
		};
	}

	VkPipelineInputAssemblyStateCreateInfo VulkanGraphicsPipeline::createInputAssemblyStateInfo()
	{
		return VkPipelineInputAssemblyStateCreateInfo{
			VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
			nullptr, // pNext
			0, // flags
			VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, // TODO: Parameterize
			VK_FALSE // TODO: Parameterize
		};
	}

	VkPipelineTessellationStateCreateInfo VulkanGraphicsPipeline::createTesselationStateInfo(U32 patchControlPoints)
	{
		return VkPipelineTessellationStateCreateInfo{
			VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO,
			nullptr, // pNext
			0, // flags
			patchControlPoints
		};
	}

	VkPipelineViewportStateCreateInfo VulkanGraphicsPipeline::createViewportStateInfo(U32 viewportCount, const VkViewport* viewports, U32 scissorCount, const VkRect2D* scissors)
	{
		return VkPipelineViewportStateCreateInfo{
			VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
			nullptr, // pNext
			0, // flags
			viewportCount,
			viewports,
			scissorCount,
			scissors
		};
	}

	VkPipelineRasterizationStateCreateInfo VulkanGraphicsPipeline::createRasterizationStateInfo(VkBool32 depthClampEnable, VkBool32 rasterizerDiscardEnable, const VkPolygonMode& polygonMode, const VkCullModeFlags& cullMode, VkBool32 depthBiasEnable, F32 depthBiasConstantFactor, F32 depthBiasClamp, F32 depthBiasSlopeFactor)
	{
		return VkPipelineRasterizationStateCreateInfo{
			VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
			nullptr, // pNext
			0, // flags
			depthClampEnable,
			rasterizerDiscardEnable,
			polygonMode,
			cullMode,
			VK_FRONT_FACE_COUNTER_CLOCKWISE,
			depthBiasEnable,
			depthBiasConstantFactor,
			depthBiasClamp,
			depthBiasSlopeFactor,
			1.0f // lineWidth // TODO: Parameterize with check for support?
		};
	}

	VkPipelineMultisampleStateCreateInfo VulkanGraphicsPipeline::createMultisampleStateInfo(const VkSampleCountFlagBits& rasterizationSamples, VkBool32 sampleShadingEnable, F32 minSampleShading, const VkSampleMask* sampleMask, VkBool32 alphaToCoverageEnable, VkBool32 alphaToOneEnable)
	{
		return VkPipelineMultisampleStateCreateInfo{
			VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
			nullptr, // pNext
			0, // flags
			rasterizationSamples,
			sampleShadingEnable,
			minSampleShading,
			sampleMask,
			alphaToCoverageEnable,
			alphaToOneEnable
		};
	}

	VkPipelineDepthStencilStateCreateInfo VulkanGraphicsPipeline::createDepthStencilStateInfo(VkBool32 depthTestEnable, VkBool32 depthWriteEnable, const VkCompareOp& depthCompareOp, VkBool32 depthBoundsTestEnable, VkBool32 stencilTestEnable, const VkStencilOpState& front, const VkStencilOpState& back, F32 minDepthBounds, F32 maxDepthBounds)
	{
		return VkPipelineDepthStencilStateCreateInfo{
			VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
			nullptr, // pNext
			0, // flags
			depthTestEnable,
			depthWriteEnable,
			depthCompareOp,
			depthBoundsTestEnable,
			stencilTestEnable,
			front,
			back,
			minDepthBounds,
			maxDepthBounds
		};
	}

	VkPipelineColorBlendAttachmentState VulkanGraphicsPipeline::createColorBlendAttachmentStateInfo(VkBool32 blendEnable, const VkBlendFactor& srcColorBlendFactor, const VkBlendFactor& dstColorBlendFactor, const VkBlendOp& colorBlendOp, const VkBlendFactor& srcAlphaBlendFactor, const VkBlendFactor& dstAlphaBlendFactor, const VkBlendOp& alphaBlendOp, const VkColorComponentFlags& colorWriteMask)
	{
		return VkPipelineColorBlendAttachmentState{
			blendEnable,
			srcColorBlendFactor,
			dstColorBlendFactor,
			colorBlendOp,
			srcAlphaBlendFactor,
			dstAlphaBlendFactor,
			alphaBlendOp,
			colorWriteMask
		};
	}

	VkPipelineColorBlendStateCreateInfo VulkanGraphicsPipeline::createColorBlendStateInfo(VkBool32 logicOpEnable, const VkLogicOp& logicOp, U32 attachmentCount, const VkPipelineColorBlendAttachmentState* attachments, F32* blendConstants)
	{
		VkPipelineColorBlendStateCreateInfo pipelineColorBlendStateInfo{
			VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
			nullptr, // pNext
			0, // flags
			logicOpEnable,
			logicOp,
			attachmentCount,
			attachments,
			0.0f
		};
		pipelineColorBlendStateInfo.blendConstants[0] = blendConstants[0];
		pipelineColorBlendStateInfo.blendConstants[1] = blendConstants[1];
		pipelineColorBlendStateInfo.blendConstants[2] = blendConstants[2];
		pipelineColorBlendStateInfo.blendConstants[3] = blendConstants[3];
		return pipelineColorBlendStateInfo;
	}

	VkPipelineDynamicStateCreateInfo VulkanGraphicsPipeline::createDynamicStateInfo(U32 dynamicStateCount, const VkDynamicState* dynamicStates)
	{
		return VkPipelineDynamicStateCreateInfo{
			VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
			nullptr, // pNext
			0, // flags
			dynamicStateCount,
			dynamicStates
		};
	}		
}