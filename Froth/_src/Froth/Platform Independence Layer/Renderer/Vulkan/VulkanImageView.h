#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "VulkanDevice.h"
#include "VulkanImage.h"

namespace Froth
{
	class VulkanImageView
	{
	public:
		VulkanImageView(const VulkanDevice& device, const VkImage& image, const VkImageViewType& viewType, const VkFormat& format, U32 baseMipLevel = 0, U32 levelCount = 1, U32 baseArrayLayer = 0, U32 layerCount = 1);
		VulkanImageView(VulkanImageView& other) = delete;
		VulkanImageView(VulkanImageView&& other);
		~VulkanImageView();
		void destroyImageView();

		VulkanImageView& operator=(VulkanImageView&& other);

		const VkImageView& getImageView() const { return m_ImageView; }

	private:
		const VulkanDevice* m_Device; // HACK: Device not owned by swap chain, used for cleanup
		VkImageView m_ImageView;
	};
}
