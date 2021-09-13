#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "VulkanDevice.h"

namespace Froth
{
	class VulkanShaderModule
	{
	public:
		VulkanShaderModule() = default;
		VulkanShaderModule(const VulkanDevice& device, const size_t& codeSize, const char* code);
		VulkanShaderModule(VulkanShaderModule& other) = delete;
		VulkanShaderModule(VulkanShaderModule&& other);
		~VulkanShaderModule();

		VulkanShaderModule& operator=(VulkanShaderModule&& other);

		VkShaderModule getModule() const { return m_ShaderModule; }

	private:
		const VulkanDevice* m_Device;
		VkShaderModule m_ShaderModule;
	};
}