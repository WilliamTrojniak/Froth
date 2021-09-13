#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "VulkanDevice.h"
#include "VulkanDescriptorSetLayout.h"

namespace Froth
{
	class VulkanPipelineLayout
	{
	public:
		VulkanPipelineLayout() = default;
		VulkanPipelineLayout(const VulkanDevice& device, U32 setLayoutCount, VulkanDescriptorSetLayout* setLayouts);
		VulkanPipelineLayout(VulkanPipelineLayout& other) = delete;
		VulkanPipelineLayout(VulkanPipelineLayout&& other);
		~VulkanPipelineLayout();

		VulkanPipelineLayout& operator=(VulkanPipelineLayout&& other);

		VkPipelineLayout getLayout() const { return m_PipelineLayout; }

	private:
		const VulkanDevice* m_Device;
		VkPipelineLayout m_PipelineLayout;
	};
}