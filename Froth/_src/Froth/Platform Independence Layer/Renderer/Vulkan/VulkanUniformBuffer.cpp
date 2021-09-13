#include "frothpch.h"
#include "VulkanUniformBuffer.h"

namespace Froth
{
	VulkanUniformBuffer::VulkanUniformBuffer(const VulkanDevice& device, const U64& bufferSize)
		: VulkanBuffer(device, bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, (VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT))
	{
	}


	void VulkanUniformBuffer::updateData(const VkDeviceSize& size, void* data) const
	{
		void* mappedData;
		vkMapMemory(m_Device->getDevice(), m_Memory, 0, size, 0, &mappedData);
		memcpy(mappedData, data, size);
		vkUnmapMemory(m_Device->getDevice(), m_Memory);
	}

}