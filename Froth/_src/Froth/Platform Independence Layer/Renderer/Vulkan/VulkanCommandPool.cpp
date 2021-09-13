#include <frothpch.h>
#include "VulkanCommandPool.h"

namespace Froth
{
	VulkanCommandPool::VulkanCommandPool(const VulkanDevice& device, U32 queueFamilyIndex)
	{
		m_Device = &device;

		VkCommandPoolCreateInfo commandPoolInfo{};
		commandPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		commandPoolInfo.pNext = nullptr;
		commandPoolInfo.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT | VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		commandPoolInfo.queueFamilyIndex = queueFamilyIndex;

		// TODO: Assert
		if (vkCreateCommandPool(m_Device->getDevice(), &commandPoolInfo, nullptr, &m_CommandPool) != VK_SUCCESS) std::cout << "Failed to create command pool!" << std::endl;
	}

	VulkanCommandPool::VulkanCommandPool(VulkanCommandPool&& other)
	{
		m_Device = other.m_Device;
		m_CommandPool = other.m_CommandPool;
		other.m_CommandPool = nullptr;
	}

	VulkanCommandPool::~VulkanCommandPool()
	{
		if (m_CommandPool != nullptr)
		{
			std::cout << "CommandPool destroyed" << std::endl;
			vkDestroyCommandPool(m_Device->getDevice(), m_CommandPool, nullptr);
		}
	}

	VulkanCommandPool& VulkanCommandPool::operator=(VulkanCommandPool&& other)
	{
		m_Device = other.m_Device;
		m_CommandPool = other.m_CommandPool;
		other.m_CommandPool = nullptr;
		return *this;
	}

}