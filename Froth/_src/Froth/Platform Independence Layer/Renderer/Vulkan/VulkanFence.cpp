#include <frothpch.h>
#include "VulkanFence.h"

namespace Froth
{

	VulkanFence::VulkanFence(const VulkanDevice& device)
		: m_Device(&device)
	{
		VkFenceCreateInfo fenceInfo;
		fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceInfo.pNext = nullptr;
		fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT; // Start all created flags as signaled // TODO: Parameterize

		// TODO: Assert
		if (vkCreateFence(m_Device->getDevice(), &fenceInfo, nullptr, &m_Fence) != VK_SUCCESS) std::cout << "Failed to create fence" << std::endl;
	}

	VulkanFence::VulkanFence(VulkanFence&& other)
		: m_Device(other.m_Device), m_Fence(other.m_Fence)
	{
		other.m_Fence = nullptr;
	}

	VulkanFence::~VulkanFence()
	{
		if (m_Fence != nullptr)
		{
			std::cout << "Fence destroyed" << std::endl;
			vkDestroyFence(m_Device->getDevice(), m_Fence, nullptr);
		}
	}

	Froth::VulkanFence& VulkanFence::operator=(VulkanFence&& other)
	{
		m_Device = other.m_Device;
		m_Fence = other.m_Fence;

		other.m_Fence = nullptr;
		return *this;
	}

}