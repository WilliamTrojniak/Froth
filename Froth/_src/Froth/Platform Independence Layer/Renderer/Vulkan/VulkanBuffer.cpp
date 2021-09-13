#include <frothpch.h>
#include "VulkanBuffer.h"

namespace Froth
{

	VulkanBuffer::VulkanBuffer(const VulkanDevice& device, const U64& bufferSize, const VkBufferUsageFlags& usage, const VkMemoryPropertyFlags& memoryFlags)
		: m_Device(&device), m_Size(bufferSize)
	{
		VkBufferCreateInfo bufferInfo;
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.pNext = nullptr;
		bufferInfo.flags = 0; // For use in sparse buffers
		bufferInfo.size = bufferSize; // Total size of the buffer in bytes
		bufferInfo.usage = usage; // Specifies the buffer will be used as a uniform buffer
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE; // TODO: Parameterize
		bufferInfo.queueFamilyIndexCount; // Ignored in exclusive mode
		bufferInfo.pQueueFamilyIndices; // Ignored in exclusive mode

		// TODO: Assert
		if (vkCreateBuffer(m_Device->getDevice(), &bufferInfo, nullptr, &m_Buffer) != VK_SUCCESS) std::cout << "Failed to create buffer!" << std::endl;


		VkMemoryRequirements memoryRequirements = getMemoryRequirements();
		U32 memoryTypeIndex = getSuitableMemoryIndex(memoryRequirements, memoryFlags);
		if (memoryTypeIndex == -1)
		{
			// TODO: Assert
			std::cout << "Could not find suitable memory for buffer!" << std::endl;
		}

		VkMemoryAllocateInfo memoryAllocateInfo;
		memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		memoryAllocateInfo.pNext = nullptr;
		memoryAllocateInfo.allocationSize = memoryRequirements.size;
		memoryAllocateInfo.memoryTypeIndex = memoryTypeIndex;

		// TODO: Assert
		if (vkAllocateMemory(m_Device->getDevice(), &memoryAllocateInfo, nullptr, &m_Memory) != VK_SUCCESS) std::cout << "Failed to allocate memory for the uniform buffer!" << std::endl;
		vkBindBufferMemory(m_Device->getDevice(), m_Buffer, m_Memory, 0);
	}

	VulkanBuffer::VulkanBuffer(VulkanBuffer&& other)
		: m_Device(other.m_Device), m_Buffer(other.m_Buffer), m_Size(other.m_Size), m_Memory(other.m_Memory)
	{
		other.m_Buffer = nullptr;
		other.m_Memory = nullptr;
	}
	
	VulkanBuffer::~VulkanBuffer()
	{
		if (m_Buffer != nullptr)
		{
			std::cout << "Buffer destroyed" << std::endl;
			std::cout << "Memory destroyed" << std::endl;
			vkDestroyBuffer(m_Device->getDevice(), m_Buffer, nullptr);
			vkFreeMemory(m_Device->getDevice(), m_Memory, nullptr);
		}
	}

	VulkanBuffer& VulkanBuffer::operator=(VulkanBuffer&& other)
	{
		m_Device = other.m_Device;
		m_Buffer = other.m_Buffer;
		m_Size = other.m_Size;
		m_Memory = other.m_Memory;
		other.m_Buffer = nullptr;
		other.m_Memory = nullptr;

		return *this;
	}

	VkMemoryRequirements VulkanBuffer::getMemoryRequirements() const
	{
		VkMemoryRequirements memoryRequirements;
		vkGetBufferMemoryRequirements(m_Device->getDevice(), m_Buffer, &memoryRequirements);
		return memoryRequirements;
	}

	U32 VulkanBuffer::getSuitableMemoryIndex(const VkMemoryRequirements& memoryRequirements, const VkMemoryPropertyFlags& flags) const
	{
		for (U32 i = 0; i < m_Device->getMemoryProperties().memoryTypeCount; i++)
		{
			if ((memoryRequirements.memoryTypeBits & (1 << i)) && (m_Device->getMemoryProperties().memoryTypes[i].propertyFlags & (flags)))
			{
				return i;
			}
		}
		// No suitable memory found
		return -1;
	}
}