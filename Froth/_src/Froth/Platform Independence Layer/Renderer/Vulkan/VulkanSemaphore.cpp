#include <frothpch.h>
#include "VulkanSemaphore.h"

namespace Froth
{

	VulkanSemaphore::VulkanSemaphore(const VulkanDevice& device)
		: m_Device(&device)
	{
		VkSemaphoreCreateInfo semaphoreInfo;
		semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
		semaphoreInfo.pNext = nullptr;
		semaphoreInfo.flags = 0;

		// TODO: Assert
		if (vkCreateSemaphore(m_Device->getDevice(), &semaphoreInfo, nullptr, &m_Semaphore) != VK_SUCCESS) std::cout << "Failed to create semaphore" << std::endl;
	}

	VulkanSemaphore::VulkanSemaphore(VulkanSemaphore&& other)
		: m_Device(other.m_Device), m_Semaphore(other.m_Semaphore)
	{
		other.m_Semaphore = nullptr;
	}

	VulkanSemaphore::~VulkanSemaphore()
	{
		if (m_Semaphore != nullptr)
		{
			std::cout << "Semaphore destroyed" << std::endl;
			vkDestroySemaphore(m_Device->getDevice(), m_Semaphore, nullptr);
		}
	}

	Froth::VulkanSemaphore& VulkanSemaphore::operator=(VulkanSemaphore&& other)
	{
		m_Device = other.m_Device;
		m_Semaphore = other.m_Semaphore;

		other.m_Semaphore = nullptr;
		return *this;
	}

}