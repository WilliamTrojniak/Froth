#pragma once
#include "VulkanDevice.h"

namespace Froth
{
	class VulkanBuffer
	{
	public:
		VulkanBuffer() = default;
		VulkanBuffer(const VulkanDevice& device, const U64& bufferSize, const VkBufferUsageFlags& usage, const VkMemoryPropertyFlags& memoryFlags);
		VulkanBuffer(VulkanBuffer& other) = delete;
		VulkanBuffer(VulkanBuffer&& other);
		~VulkanBuffer();

		VulkanBuffer& operator=(VulkanBuffer&& other);

		VkMemoryRequirements getMemoryRequirements() const;
		U32 getSuitableMemoryIndex(const VkMemoryRequirements& memoryRequirements, const VkMemoryPropertyFlags& flags) const; // Returns -1 if no suitable memory found

		const VkBuffer& getBuffer() const { return m_Buffer; }

	protected:
		const VulkanDevice* m_Device;
		VkBuffer m_Buffer;
		U64 m_Size;
		VkDeviceMemory m_Memory;
	};
}