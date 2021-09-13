#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "VulkanDevice.h"

namespace Froth
{
	class VulkanImage
	{
	public:
		VulkanImage(const VulkanDevice& device, const VkImageType& imageType, const VkFormat& imageFormat, U32 imageWidth, U32 imageHeight, U32 imageDepth = 1, U32 imageMipLevels = 1);
		VulkanImage(VulkanImage& other) = delete;
		VulkanImage(VulkanImage&& other);
		~VulkanImage();


		const VkImage& getImage() const { return m_Image; }

	private:
		const VulkanDevice* m_Device; // HACK: Device not owned by swap chain, used for cleanup
		VkImage m_Image;
	};
}