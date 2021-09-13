#include <frothpch.h>
#include "VulkanPipelineLayout.h"

namespace Froth
{

	VulkanPipelineLayout::VulkanPipelineLayout(const VulkanDevice& device, U32 setLayoutCount, VulkanDescriptorSetLayout* setLayouts)
		: m_Device(&device)
	{
		std::vector<VkDescriptorSetLayout> descriptorSets(setLayoutCount);
		for (U32 i = 0; i < setLayoutCount; i++)
		{
			descriptorSets[i] = setLayouts[i].getDescriptorSetLayout();
		}

		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = setLayoutCount;
		pipelineLayoutInfo.pSetLayouts = descriptorSets.data();
		pipelineLayoutInfo.pushConstantRangeCount = 0; // TODO: Parameterize
		pipelineLayoutInfo.pPushConstantRanges = nullptr; // TODO: Parameterize

		// TODO: Assert
		if (vkCreatePipelineLayout(m_Device->getDevice(), &pipelineLayoutInfo, nullptr, &m_PipelineLayout) != VK_SUCCESS) std::cout << "Failed to create pipeline layout!" << std::endl;
	}

	VulkanPipelineLayout::VulkanPipelineLayout(VulkanPipelineLayout&& other)
		: m_Device(other.m_Device), m_PipelineLayout(other.m_PipelineLayout)
	{
		other.m_PipelineLayout = nullptr;
	}

	VulkanPipelineLayout::~VulkanPipelineLayout()
	{
		if (m_PipelineLayout != nullptr)
		{
			std::cout << "PipelineLayout destroyed" << std::endl;
			vkDestroyPipelineLayout(m_Device->getDevice(), m_PipelineLayout, nullptr);
		}
	}

	VulkanPipelineLayout& VulkanPipelineLayout::operator=(VulkanPipelineLayout&& other)
	{
		m_Device = other.m_Device;
		m_PipelineLayout = other.m_PipelineLayout;

		other.m_PipelineLayout = nullptr;
		return *this;
	}

}