#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "VulkanDevice.h"

namespace Froth
{
	class VulkanCommandPool
	{
	public:
		VulkanCommandPool() = default;
		VulkanCommandPool(const VulkanDevice& device, U32 queueFamilyIndex);
		VulkanCommandPool(VulkanCommandPool& other) = delete;
		VulkanCommandPool(VulkanCommandPool&& other);
		~VulkanCommandPool();
		VulkanCommandPool& operator=(VulkanCommandPool&& other);
	
		const VkCommandPool& getCommandPool() const { return m_CommandPool; }
	private:
		const VulkanDevice* m_Device; // HACK: Device not owned by swap chain, used for cleanup
		VkCommandPool m_CommandPool;
	};

	

}