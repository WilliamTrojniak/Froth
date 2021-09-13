#include <frothpch.h>
#include "VulkanVertexBuffer.h"

namespace Froth
{

	VulkanVertexBuffer::VulkanVertexBuffer(const VulkanDevice& device, U32 vertexSize, U32 vertexCount)
		: VulkanBuffer(device, (U64)vertexSize * (U64)vertexCount, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, (VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT))
	{
	}

	void VulkanVertexBuffer::updateData(U32 vertexCount, const VkDeviceSize& vertexSize, void* data) const
	{
		void* mappedData;
		VkDeviceSize size = vertexCount * vertexSize;
		vkMapMemory(m_Device->getDevice(), m_Memory, 0, size, 0, &mappedData);
		memcpy(mappedData, data, size);
		vkUnmapMemory(m_Device->getDevice(), m_Memory);
	}

}