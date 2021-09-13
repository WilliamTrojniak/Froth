#pragma once
#include "VulkanBuffer.h"

namespace Froth
{
	class VulkanUniformBuffer : public VulkanBuffer
	{
	public:
		VulkanUniformBuffer() = default;
		VulkanUniformBuffer(const VulkanDevice& device, const U64& bufferSize);

		void updateData(const VkDeviceSize& size, void* data) const;
	};

	

}