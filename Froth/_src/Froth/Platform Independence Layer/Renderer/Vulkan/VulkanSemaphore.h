#pragma once
#include "VulkanDevice.h"

namespace Froth
{
	class VulkanSemaphore
	{
	public:
		VulkanSemaphore() = default;
		VulkanSemaphore(const VulkanDevice& device);
		VulkanSemaphore(VulkanSemaphore& other) = delete;
		VulkanSemaphore(VulkanSemaphore&& other);
		~VulkanSemaphore();
		VulkanSemaphore& operator=(VulkanSemaphore&& other);

		const VkSemaphore& getSemaphore() const { return m_Semaphore; }

	private:
		const VulkanDevice* m_Device;
		VkSemaphore m_Semaphore;
	};
}