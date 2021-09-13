#pragma once
#include "VulkanBuffer.h"

namespace Froth
{
	class VulkanIndexBuffer : public VulkanBuffer
	{
	public:
		VulkanIndexBuffer() = default;
		VulkanIndexBuffer(const VulkanDevice& device, U32 indexCount);

		void updateData(U32 indexCount, void* data) const;
		
	};
}