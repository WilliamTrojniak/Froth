#pragma once
#include "VulkanDevice.h"

namespace Froth
{
	class VulkanFence
	{
	public:
		VulkanFence() = default;
		VulkanFence(const VulkanDevice& device);
		VulkanFence(VulkanFence& other) = delete;
		VulkanFence(VulkanFence&& other);
		~VulkanFence();
		VulkanFence& operator=(VulkanFence&& other);

		const VkFence& getFence() const { return m_Fence; }

	private:
		const VulkanDevice* m_Device;
		VkFence m_Fence;
	};
}
