#include <frothpch.h>
#include "VulkanDescriptorPool.h"

namespace Froth
{
	VulkanDescriptorPool::VulkanDescriptorPool(const VulkanDevice& device, U32 maxSets, U32 poolSizeCount, VkDescriptorPoolSize* poolSizes)
		: m_Device(&device)
	{
		VkDescriptorPoolCreateInfo descriptorPoolInfo;
		descriptorPoolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		descriptorPoolInfo.pNext = nullptr;
		descriptorPoolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
		descriptorPoolInfo.maxSets = maxSets;
		descriptorPoolInfo.poolSizeCount = poolSizeCount;
		descriptorPoolInfo.pPoolSizes = poolSizes;

		// TODO: Assert
		if (vkCreateDescriptorPool(m_Device->getDevice(), &descriptorPoolInfo, nullptr, &m_DescriptorPool) != VK_SUCCESS) std::cout << "Failed to create descriptor pool!" << std::endl;
	}

	VulkanDescriptorPool::VulkanDescriptorPool(VulkanDescriptorPool&& other)
	{
		m_Device = other.m_Device;
		m_DescriptorPool = other.m_DescriptorPool;

		other.m_DescriptorPool = nullptr;
	}

	VulkanDescriptorPool::~VulkanDescriptorPool()
	{
		if (m_DescriptorPool != nullptr)
		{
			std::cout << "DescriptorPool destroyed" << std::endl;
			vkDestroyDescriptorPool(m_Device->getDevice(), m_DescriptorPool, nullptr);
		}
	}

	VulkanDescriptorPool& VulkanDescriptorPool::operator=(VulkanDescriptorPool&& other)
	{
		m_Device = other.m_Device;
		m_DescriptorPool = other.m_DescriptorPool;

		other.m_DescriptorPool = nullptr;
		return *this;
	}

	VkDescriptorPoolSize VulkanDescriptorPool::createDescriptorPoolSize(const VkDescriptorType& type, U32 size)
	{
		return VkDescriptorPoolSize {
			type,
			size
		};
	}

}