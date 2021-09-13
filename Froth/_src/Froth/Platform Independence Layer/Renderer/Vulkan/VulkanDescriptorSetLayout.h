#pragma once 
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "VulkanDevice.h"

namespace Froth
{
	class VulkanDescriptorSetLayout
	{
	public:
		VulkanDescriptorSetLayout() = default;
		VulkanDescriptorSetLayout(const VulkanDevice& device, U32 bindingCount, VkDescriptorSetLayoutBinding* bindings);
		VulkanDescriptorSetLayout(VulkanDescriptorSetLayout& other) = delete;
		VulkanDescriptorSetLayout(VulkanDescriptorSetLayout&& other);
		~VulkanDescriptorSetLayout();

		VulkanDescriptorSetLayout& operator=(VulkanDescriptorSetLayout&& other);

		const VkDescriptorSetLayout& getDescriptorSetLayout() const { return m_DescriptorSetLayout; }

		static VkDescriptorSetLayoutBinding createDescriptorSetLayoutBinding(U32 binding, const VkDescriptorType& descriptorType, U32 descriptorCount, const VkShaderStageFlags& stageFlags, VkSampler* immutableSamplers = nullptr);
	private:
		const VulkanDevice* m_Device;
		VkDescriptorSetLayout m_DescriptorSetLayout;
	};
}