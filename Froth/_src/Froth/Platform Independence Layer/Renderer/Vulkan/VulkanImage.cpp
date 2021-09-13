#include <frothpch.h>
#include "VulkanImage.h"

namespace Froth
{

	VulkanImage::VulkanImage(const VulkanDevice& device, const VkImageType& imageType, const VkFormat& imageFormat, U32 imageWidth, U32 imageHeight, U32 imageDepth, U32 imageMipLevels)
	{
		m_Device = &device;

		VkImageCreateInfo imageInfo;
		imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		imageInfo.pNext = nullptr;
		imageInfo.flags = 0;
		imageInfo.imageType = imageType;
		imageInfo.format = imageFormat;
		imageInfo.extent.width = imageWidth;
		imageInfo.extent.height = imageHeight;
		imageInfo.extent.depth = imageDepth;
		imageInfo.mipLevels = imageMipLevels;
		imageInfo.arrayLayers = 1; // TODO: Parameterize
		imageInfo.samples = VK_SAMPLE_COUNT_1_BIT; // TODO: Parameterize
		imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL; // TODO: Parameterize (Optimal layout, non-linear access)
		imageInfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT; // TODO: Parameterize
		imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE; // TODO: Change
		imageInfo.queueFamilyIndexCount = 0;
		imageInfo.pQueueFamilyIndices = nullptr;
		imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
 
		// TODO: Assert
		if (vkCreateImage(device.getDevice(), &imageInfo, nullptr, &m_Image) != VK_SUCCESS) std::cout << "Failed to create texture image" << std::endl;

// 		VkMemoryRequirements imageMemoryRequirements;
// 		vkGetImageMemoryRequirements(m_Device->getDevice(), m_Image, &imageMemoryRequirements);
// 
// 		U32 imageMemoryTypeIndex = -1;
// 		VkPhysicalDeviceMemoryProperties m = m_Device->getMemoryProperties();
// 		for (U32 i = 0; i < m_Device->getMemoryProperties().memoryTypeCount; i++)
// 		{
// 			if ((imageMemoryRequirements.memoryTypeBits & (1 << i)) && (m_Device->getMemoryProperties().memoryTypes[i].propertyFlags & (VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)))
// 			{
// 				imageMemoryTypeIndex = i;
// 				break;
// 			}
// 		}
// 
// 		if (imageMemoryTypeIndex == -1)
// 		{
// 			// TODO: Assert
// 			std::cout << "Failed to find suitable memory for image!" << std::endl;
// 		}
// 
// 		VkMemoryAllocateInfo imageAllocInfo{};
// 		imageAllocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
// 		imageAllocInfo.pNext = nullptr;
// 		imageAllocInfo.allocationSize = imageMemoryRequirements.size;
// 		imageAllocInfo.memoryTypeIndex = imageMemoryTypeIndex;
// 
// 		if (vkAllocateMemory(m_Device->getDevice(), &imageAllocInfo, nullptr, &m_Memory) != VK_SUCCESS) std::cout << "Failed to allocate memory for the texture image" << std::endl;
// 
// 		vkBindImageMemory(m_Device->getDevice(), m_Image, m_Memory, 0);
		
		// TODO: Move to VulkanRenderer.cpp

// 		VkCommandBufferAllocateInfo imageCommandBufferAllocInfo;
// 		imageCommandBufferAllocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
// 		imageCommandBufferAllocInfo.pNext = nullptr;
// 		imageCommandBufferAllocInfo.commandPool = commandPool;
// 		imageCommandBufferAllocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
// 		imageCommandBufferAllocInfo.commandBufferCount = 1;
// 
// 		VkCommandBuffer textureImageCommandBuffer;
// 		vkAllocateCommandBuffers(logicDevice, &imageCommandBufferAllocInfo, &textureImageCommandBuffer);
// 
// 		VkCommandBufferBeginInfo textureImageBufferBeginInfo{};
// 		textureImageBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
// 		textureImageBufferBeginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
// 
// 		vkBeginCommandBuffer(textureImageCommandBuffer, &textureImageBufferBeginInfo);
// 
// 		VkImageMemoryBarrier imageBarrier{};
// 		imageBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
// 		imageBarrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
// 		imageBarrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
// 		imageBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
// 		imageBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
// 		imageBarrier.image = textureImage;
// 		imageBarrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
// 		imageBarrier.subresourceRange.baseMipLevel = 0;
// 		imageBarrier.subresourceRange.levelCount = 1;
// 		imageBarrier.subresourceRange.baseArrayLayer = 0;
// 		imageBarrier.subresourceRange.layerCount = 1;
// 		imageBarrier.srcAccessMask = 0; // TODO
// 		imageBarrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT; // TODO
// 
// 		vkCmdPipelineBarrier(textureImageCommandBuffer, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0, nullptr, 1, &imageBarrier);
// 
// 		VkBufferImageCopy textureImageCopyRegion{};
// 		textureImageCopyRegion.bufferOffset = 0;
// 		textureImageCopyRegion.bufferRowLength = 0;
// 		textureImageCopyRegion.bufferImageHeight = 0;
// 		textureImageCopyRegion.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
// 		textureImageCopyRegion.imageSubresource.mipLevel = 0;
// 		textureImageCopyRegion.imageSubresource.baseArrayLayer = 0;
// 		textureImageCopyRegion.imageSubresource.layerCount = 1;
// 		textureImageCopyRegion.imageOffset = { 0, 0, 0 };
// 		textureImageCopyRegion.imageExtent = { static_cast<uint32_t>(textureWidth), static_cast<uint32_t>(textureHeight), 1 };
// 
// 		vkCmdCopyBufferToImage(textureImageCommandBuffer, imageStagingBuffer, textureImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &textureImageCopyRegion);
// 
// 		VkImageMemoryBarrier imageBarrierSecond{};
// 		imageBarrierSecond.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
// 		imageBarrierSecond.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
// 		imageBarrierSecond.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
// 		imageBarrierSecond.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
// 		imageBarrierSecond.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
// 		imageBarrierSecond.image = textureImage;
// 		imageBarrierSecond.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
// 		imageBarrierSecond.subresourceRange.baseMipLevel = 0;
// 		imageBarrierSecond.subresourceRange.levelCount = 1;
// 		imageBarrierSecond.subresourceRange.baseArrayLayer = 0;
// 		imageBarrierSecond.subresourceRange.layerCount = 1;
// 		imageBarrierSecond.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT; // TODO
// 		imageBarrierSecond.dstAccessMask = VK_ACCESS_SHADER_READ_BIT; // TODO
// 
// 		vkCmdPipelineBarrier(textureImageCommandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, nullptr, 0, nullptr, 1, &imageBarrierSecond);
// 
// 		vkEndCommandBuffer(textureImageCommandBuffer);
// 		VkSubmitInfo textureImageCommandBufferSubmitInfo{};
// 		textureImageCommandBufferSubmitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
// 		textureImageCommandBufferSubmitInfo.commandBufferCount = 1;
// 		textureImageCommandBufferSubmitInfo.pCommandBuffers = &textureImageCommandBuffer;
// 
// 		vkQueueSubmit(graphicsQueue, 1, &textureImageCommandBufferSubmitInfo, VK_NULL_HANDLE);
// 		vkQueueWaitIdle(graphicsQueue);
// 		vkFreeCommandBuffers(logicDevice, commandPool, 1, &textureImageCommandBuffer);
// 		vkDestroyBuffer(logicDevice, imageStagingBuffer, nullptr);
// 		vkFreeMemory(logicDevice, imageStagingBufferMemory, nullptr);
	}

	VulkanImage::VulkanImage(VulkanImage&& other)
	{
		m_Device = other.m_Device;
		m_Image = other.m_Image;

		other.m_Image = nullptr;
	}

	VulkanImage::~VulkanImage()
	{
		if (m_Image != nullptr)
		{
			std::cout << "Image destroyed" << std::endl;
			vkDestroyImage(m_Device->getDevice(), m_Image, nullptr);
		}
	}

}