#pragma once
#include "VulkanDevice.h"
#include "VulkanDescriptorPool.h"
#include "VulkanDescriptorSetLayout.h"

namespace Froth
{
	class VulkanDescriptorSet
	{
	public:
		VulkanDescriptorSet() = default;
		VulkanDescriptorSet(const VulkanDevice& device, const VulkanDescriptorPool& descriptorPool, const VulkanDescriptorSetLayout& setLayout);
		VulkanDescriptorSet(VulkanDescriptorSet& other) = delete;
		VulkanDescriptorSet(VulkanDescriptorSet&& other);
		~VulkanDescriptorSet();

		VulkanDescriptorSet& operator=(VulkanDescriptorSet&& other);

		const VkDescriptorSet& getSet() const { return m_DescriptorSet; }

	private:
		const VulkanDevice* m_Device;
		const VulkanDescriptorPool* m_Pool;
		VkDescriptorSet m_DescriptorSet;
	};
}