#include <frothpch.h>
#include "VulkanDescriptorSetLayout.h"

namespace Froth
{

	VulkanDescriptorSetLayout::VulkanDescriptorSetLayout(const VulkanDevice& device, U32 bindingCount, VkDescriptorSetLayoutBinding* bindings)
		: m_Device(&device)
	{

		VkDescriptorSetLayoutCreateInfo descriptorSetLayoutInfo;
		descriptorSetLayoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		descriptorSetLayoutInfo.pNext = nullptr;
		descriptorSetLayoutInfo.flags = 0;
		descriptorSetLayoutInfo.bindingCount = bindingCount;
		descriptorSetLayoutInfo.pBindings = bindings;

		// TODO: Assert
		if (vkCreateDescriptorSetLayout(m_Device->getDevice(), &descriptorSetLayoutInfo, nullptr, &m_DescriptorSetLayout) != VK_SUCCESS) std::cout << "Failed to create descriptor set layout for uniform buffer" << std::endl;
	}

	VulkanDescriptorSetLayout::VulkanDescriptorSetLayout(VulkanDescriptorSetLayout&& other)
	{
		m_Device = other.m_Device;
		m_DescriptorSetLayout = other.m_DescriptorSetLayout;

		other.m_DescriptorSetLayout = nullptr;
	}

	VulkanDescriptorSetLayout::~VulkanDescriptorSetLayout()
	{
		if (m_DescriptorSetLayout != nullptr)
		{
			std::cout << "DescriptorSetLayout destroyed" << std::endl;
			vkDestroyDescriptorSetLayout(m_Device->getDevice(), m_DescriptorSetLayout, nullptr);
		}
	}

	VulkanDescriptorSetLayout& VulkanDescriptorSetLayout::operator=(VulkanDescriptorSetLayout&& other)
	{
		m_Device = other.m_Device;
		m_DescriptorSetLayout = other.m_DescriptorSetLayout;

		other.m_DescriptorSetLayout = nullptr;
		return *this;
	}

	VkDescriptorSetLayoutBinding VulkanDescriptorSetLayout::createDescriptorSetLayoutBinding(U32 binding, const VkDescriptorType& descriptorType, U32 descriptorCount, const VkShaderStageFlags& stageFlags, VkSampler* immutableSamplers)
	{
		return VkDescriptorSetLayoutBinding {
			binding, // Same as in shader
			descriptorType,
			descriptorCount,
			stageFlags, // Stages in the pipeline that it is accessible
			immutableSamplers // Optional - only affects image sampling buffers
		};
	}

}