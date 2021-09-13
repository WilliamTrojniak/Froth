#include <frothpch.h>
#include "VulkanIndexBuffer.h"

namespace Froth
{

	VulkanIndexBuffer::VulkanIndexBuffer(const VulkanDevice& device, U32 indexCount)
		: VulkanBuffer(device, indexCount * sizeof(U16), VK_BUFFER_USAGE_INDEX_BUFFER_BIT, (VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT|VK_MEMORY_PROPERTY_HOST_COHERENT_BIT))
	{

	}

	void VulkanIndexBuffer::updateData(U32 indexCount, void* data) const
	{
		void* mappedData;
		VkDeviceSize size = indexCount * sizeof(U16);
		vkMapMemory(m_Device->getDevice(), m_Memory, 0, size, 0, &mappedData);
		memcpy(mappedData, data, size);
		vkUnmapMemory(m_Device->getDevice(), m_Memory);
	}

}