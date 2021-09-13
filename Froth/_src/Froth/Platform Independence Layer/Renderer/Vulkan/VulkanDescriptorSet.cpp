#include <frothpch.h>
#include "VulkanDescriptorSet.h"


namespace Froth
{

	VulkanDescriptorSet::VulkanDescriptorSet(const VulkanDevice& device, const VulkanDescriptorPool& descriptorPool, const VulkanDescriptorSetLayout& setLayout)
		: m_Device(&device), m_Pool(&descriptorPool)
	{
		VkDescriptorSetAllocateInfo descriptorSetAllocateInfo;
		descriptorSetAllocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		descriptorSetAllocateInfo.pNext = nullptr;
		descriptorSetAllocateInfo.descriptorPool = m_Pool->getPool();
		descriptorSetAllocateInfo.descriptorSetCount = 1; // TODO: Parameterize to create multiple descriptor sets at a time
		descriptorSetAllocateInfo.pSetLayouts = &setLayout.getDescriptorSetLayout();

		// TODO: Assert
		if (vkAllocateDescriptorSets(m_Device->getDevice(), &descriptorSetAllocateInfo, &m_DescriptorSet) != VK_SUCCESS) std::cout << "Failed to allocate descriptor set" << std::endl;
	}

	VulkanDescriptorSet::VulkanDescriptorSet(VulkanDescriptorSet&& other)
		: m_Device(other.m_Device), m_Pool(other.m_Pool), m_DescriptorSet(other.m_DescriptorSet)
	{
		other.m_DescriptorSet = nullptr;
	}

	VulkanDescriptorSet::~VulkanDescriptorSet()
	{
		if (m_DescriptorSet != nullptr)
		{
			std::cout << "DescriptorSet destroyed" << std::endl;
			vkFreeDescriptorSets(m_Device->getDevice(), m_Pool->getPool(), 1, &m_DescriptorSet);
		}
	}

	VulkanDescriptorSet& VulkanDescriptorSet::operator=(VulkanDescriptorSet&& other)
	{
		m_Device = other.m_Device;
		m_Pool = other.m_Pool;
		m_DescriptorSet = other.m_DescriptorSet;

		other.m_DescriptorSet = nullptr;
		return *this;
	}

}