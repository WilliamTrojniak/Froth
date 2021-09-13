#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "VulkanDevice.h"

namespace Froth
{
	class VulkanDescriptorPool
	{
	public:
		VulkanDescriptorPool() = default;
		VulkanDescriptorPool(const VulkanDevice& device, U32 maxSets, U32 poolSizeCount, VkDescriptorPoolSize* poolSizes);
		VulkanDescriptorPool(VulkanDescriptorPool& other) = delete;
		VulkanDescriptorPool(VulkanDescriptorPool&& other);
		~VulkanDescriptorPool();

		VulkanDescriptorPool& operator=(VulkanDescriptorPool&& other);

		const VkDescriptorPool& getPool() const { return m_DescriptorPool; }

		static VkDescriptorPoolSize createDescriptorPoolSize(const VkDescriptorType& type, U32 size);

	private:
		const VulkanDevice* m_Device;
		VkDescriptorPool m_DescriptorPool;

	};
}