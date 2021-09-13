#include <frothpch.h>
#include "VulkanShaderModule.h"

namespace Froth
{
	
	VulkanShaderModule::VulkanShaderModule(const VulkanDevice& device, const size_t& codeSize, const char* code)
		: m_Device(&device)
	{
		VkShaderModuleCreateInfo shaderModuleInfo;
		shaderModuleInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		shaderModuleInfo.pNext = nullptr;
		shaderModuleInfo.flags = 0;
		shaderModuleInfo.codeSize = codeSize;
		shaderModuleInfo.pCode = reinterpret_cast<const uint32_t*>(code);

		// TODO: Assert
		if (vkCreateShaderModule(m_Device->getDevice(), &shaderModuleInfo, nullptr, &m_ShaderModule) != VK_SUCCESS) std::cout << "Failed to create shader module" << std::endl;
	}

	VulkanShaderModule::VulkanShaderModule(VulkanShaderModule&& other)
	{
		m_Device = other.m_Device;
		m_ShaderModule = other.m_ShaderModule;

		other.m_ShaderModule = nullptr;
	}

	VulkanShaderModule::~VulkanShaderModule()
	{
		if (m_ShaderModule != nullptr)
		{
			std::cout << "ShaderModule destroyed" << std::endl;
			vkDestroyShaderModule(m_Device->getDevice(), m_ShaderModule, nullptr);
		}
	}

	VulkanShaderModule& VulkanShaderModule::operator=(VulkanShaderModule&& other)
	{
		m_Device = other.m_Device;
		m_ShaderModule = other.m_ShaderModule;

		other.m_ShaderModule = nullptr;
		return *this;
	}

}