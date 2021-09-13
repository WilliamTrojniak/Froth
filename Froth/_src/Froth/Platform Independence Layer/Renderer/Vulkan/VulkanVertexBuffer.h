#pragma once
#include "VulkanBuffer.h"

namespace Froth
{
	class VulkanVertexBuffer : public VulkanBuffer
	{
	public:
		VulkanVertexBuffer() = default;
		VulkanVertexBuffer(const VulkanDevice& device, U32 vertexSize, U32 vertexCount);

		void updateData(U32 vertexCount, const VkDeviceSize& vertexSize, void* data) const;
	};
}
